/****************************************************************************
**
** Copyright (C) 2010-2016 B.D. Mihai.
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
#include "EventService.h"

/*!
Create a new instance of the EventService class.
*/
EventService::EventService(QObject *parent, AuthService *auth) : QObject(parent)
{
  this->auth = auth;

  connect(&netAccMan, SIGNAL(finished(QNetworkReply *)),
          this, SLOT(getEventsFinished(QNetworkReply *)));
  connect(this, SIGNAL(getNextEvents(const QString &, const QString &, const QString &, const QString &)),
          this, SLOT(getEvents(const QString &, const QString &, const QString &, const QString &)), Qt::QueuedConnection);
}

/*!
Clean up.
*/
EventService::~EventService()
{
}

/*!
Get the events from Google.
/param pageToken Next page token for handling more events.
*/
void EventService::getEvents(const QString &calendarId, const QString &start, const QString &end, const QString &pageToken)
{
  /*! This endpoint returns entries on the user's calendar list.*/
  const QString endpoint = "https://www.googleapis.com/calendar/v3/calendars/%1/events";

  /*! Maximum number of events returned on one result page. By default the
  value is 250 events. The page size can never be larger than 2500 events.
  Optional. (integer, 1+) */
  const QString maxResults = "200";

  /*! Selector specifying which fields to include in a partial response. */
  const QString fields = "items(id,summary,start,end),nextPageToken";

  /*! Whether to expand recurring events into instances and only return single
  one-off events and instances of recurring events, but not the underlying
  recurring events themselves. Optional. The default is False. (boolean) */
  const QString singleEvents = "true";

  // build up the query for url
  QUrlQuery urlQuery;
  urlQuery.addQueryItem("maxResults", maxResults);
  urlQuery.addQueryItem("fields", fields);
  urlQuery.addQueryItem("singleEvents", singleEvents);
  urlQuery.addQueryItem("timeMin", start);
  urlQuery.addQueryItem("timeMax", end);
  if (pageToken != "")
    urlQuery.addQueryItem("pageToken", pageToken);

  // build up the url for the request
  QUrl url(QString(endpoint).arg(QString(QUrl::toPercentEncoding(calendarId))));
  url.setQuery(urlQuery);

  QString urlStr = url.toString();

  // set-up the request
  QNetworkRequest request = QNetworkRequest();
  request.setUrl(url);
  request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer ") + auth->getAccessToken().toLatin1());

  // get events request
  netAccMan.get(request);
}

/*!
Parse the received data.
*/
void EventService::getEventsFinished(QNetworkReply *reply)
{
  if (!reply->error())
  {
    QJsonParseError parserError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll(), &parserError);

    if (parserError.error == QJsonParseError::NoError)
    {
      QJsonObject jsonObject = jsonResponse.object();

      QJsonArray jsonItems = jsonObject["items"].toArray();
      foreach(const QJsonValue value, jsonItems)
      {
        EventItem eventItem(value.toObject());
        emit eventAvailable(eventItem);
      }

      QJsonValue value = jsonObject["nextPageToken"];
      if (value.isString())
      {
        QString nextPageToken = value.toString();
        QNetworkRequest request = reply->request();
        QUrl url = request.url();
        QUrlQuery urlQuery = QUrlQuery(url.query());
        QString calendarId = url.path();
        calendarId.replace("/calendar/v3/calendars/", "");
        calendarId.replace("/events", "");
        emit getNextEvents(calendarId, urlQuery.queryItemValue("timeMin"), urlQuery.queryItemValue("timeMax"), nextPageToken);
      }
    }
    else
    {
      emit error(parserError.errorString());
    }
  }
  else
  {
    emit error(reply->errorString());
  }

  reply->deleteLater();
}
