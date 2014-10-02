/****************************************************************************
**
** Copyright (C) 2010-2011 B.D. Mihai.
**
** This file is part of CalendarGadget.
**
** CalendarGadget is free software: you can redistribute it and/or modify it 
** under the terms of the GNU Lesser Public License as published by the Free 
** Software Foundation, either version 3 of the License, or (at your option) 
** any later version.
**
** CalendarGadget is distributed in the hope that it will be useful, but 
** WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
** or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for 
** more details.
**
** You should have received a copy of the GNU Lesser Public License along 
** with CalendarGadget.  If not, see http://www.gnu.org/licenses/.
**
****************************************************************************/

#include "StdAfx.h"
#include "CalendarService.h"

/*!
Create a new instance of the CalendarService class.
*/
CalendarService::CalendarService()
{
  netAccMan = new QNetworkAccessManager();
  auth = new AuthService(netAccMan, "cl");
  loop = new QEventLoop();
  serviceWorking = false;
}

/*!
Clean up.
*/
CalendarService::~CalendarService()
{
  delete auth;
  delete loop;
  delete netAccMan;
}

/*!
This function checks if the necessary conditions for using this service are
fulfilled.
*/
bool CalendarService::isServiceAvailable()
{
  return QSslSocket::supportsSsl();
}

/*!
Get the error text if it is the case.
*/
QString CalendarService::getError()
{
  return error;
}

/*!
Configure the proxy used by the service.
*/
void CalendarService::configureProxy(QNetworkProxy::ProxyType type, const QString &hostName, 
                                     quint16 port, const QString &user, const QString &password)
{
  netAccMan->setProxy(QNetworkProxy(type, hostName, port, user, password));
}

/*!
This function try to login into the Google account and obtain the authentication
code for the calendar service ("cl"). The function will block until the request 
is finalized.
\param mail the full mail address.
\param password the password for the provided mail account.
\return true if the login was performed successfully.
*/
bool CalendarService::login(QString mail, QString password)
{
  if (auth->login(mail, password))
  {
    error = "";
    serviceWorking = getSession();
  }
  else
  {
    error = auth->getError();
    serviceWorking = false;
  }

  return serviceWorking;
}

/*!
This function try to obtain a session id from Google in order to be able to 
access the calendar without redirection.
The function will block until the request is finalized.
*/
bool CalendarService::getSession()
{
  QUrl address("http://www.google.com/calendar/feeds/default/private/full");

  QNetworkRequest request = QNetworkRequest();
  request.setUrl(address);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  QString authorization = QString("GoogleLogin auth=%1").arg(auth->getAuth()).toLatin1();
  request.setRawHeader("Authorization", authorization.toLatin1());
  request.setRawHeader("GData-Version", "2.0");
  request.setRawHeader("X-If-No-Redirect", "1");

  reply = netAccMan->get(request);
  connect(reply, SIGNAL(finished()),
    this, SLOT(getSessionDone()));

  loop->exec();

  return error.isEmpty();
}

/*!
This function starts the retrieval of all-day-events from the calendar. The
function returns immediately after the request is sent. The eventsAvailable signal
will be emitted after the reply is received.
*/
void CalendarService::getEvents(const QDate &newStartDate, const QDate &newEndDate)
{
  if (serviceWorking)
  {
    QUrl address("http://www.google.com/calendar/feeds/default/private/full");
    QUrlQuery urlQuery;

    urlQuery.addQueryItem("gsessionid", session);
    urlQuery.addQueryItem("start-min", QString("%1T00:00:00").arg(newStartDate.toString("yyyy-MM-dd")));
    urlQuery.addQueryItem("start-max", QString("%1T23:59:59").arg(newEndDate.toString("yyyy-MM-dd")));

    address.setQuery(urlQuery);

    QNetworkRequest request = QNetworkRequest();
    request.setUrl(address);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString authorization = QString("GoogleLogin auth=%1").arg(auth->getAuth()).toLatin1();
    request.setRawHeader("Authorization", authorization.toLatin1());
    request.setRawHeader("GData-Version", "2.0");

    serviceWorking = false;

    reply = netAccMan->get(request);
    connect(reply, SIGNAL(finished()),
      this, SLOT(getEventsDone()));
  }
}

/*!
This function handles the reply to the get session request.
*/
void CalendarService::getSessionDone()
{
  if (reply->hasRawHeader("X-Redirect-Location")) 
  {
    session = reply->rawHeader("X-Redirect-Location");
    session = session.right(session.length() - session.indexOf('=') - 1);
  }
  else
  {
    if (reply->error())
    {
      error = reply->errorString();
    }
  }

  reply->deleteLater();
  reply = 0;
  loop->exit();
}

/*!
This function handles the reply to the get events request. After the events are
decoded from the xml atom feed the eventsAvailable signal is emitted.
*/
void CalendarService::getEventsDone()
{
  EventList events;

  if (!reply->error()) 
  {
    QString response = reply->readAll();
    QXmlStreamReader xml(response);
    QString title;
    QString startTime;
    QString endTime;
    bool readData = false;

    events.clear();
    error = "";
    while (!xml.atEnd()) 
    {
      xml.readNext();
      if (xml.isStartElement() && xml.name() == "entry")
      {
        readData = true;
      }
      if (xml.isStartElement() && xml.name() == "title" && readData)
      {
        title = xml.readElementText();
      }
      if (xml.isStartElement() && xml.qualifiedName() == "gd:when" && readData )
      {
        QXmlStreamAttributes attributes = xml.attributes();

        if (attributes.hasAttribute("startTime"))
          startTime = attributes.value("startTime").toString().simplified();

        if (attributes.hasAttribute("endTime"))
          endTime = attributes.value("endTime").toString().simplified();
      }
      if (xml.isEndElement() && xml.name() == "entry")
      {
        if (!startTime.contains('T') || !endTime.contains('T'))
        {
          QDate date = QDate::fromString(startTime, "yyyy-MM-dd");
          Event event;
          event.setTitle(title);
          event.setDate(date);
          events.append(event);
        }
        readData = false;
      }
    }
    if (xml.hasError())
    {
      error = xml.errorString();
    }
  } 
  else 
  {
    error = reply->errorString();
  }

  reply->deleteLater();
  reply = 0;

  if (error.isEmpty())
    emit eventsAvailable(events);

  serviceWorking = true;
}
