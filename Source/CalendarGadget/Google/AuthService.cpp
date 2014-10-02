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
#include "AuthService.h"

/*!
Create a new instance of the AuthService using a network access manager.
*/
AuthService::AuthService(QNetworkAccessManager *netAccMan, const QString &service)
{
  this->netAccMan = netAccMan;
  this->service = service;

  error = "";
  auth = "";
  lsid = "";
  sid = "";
  
  loop = new QEventLoop();
}

/*!
Clean up.
*/
AuthService::~AuthService()
{
  delete loop;
}

/*!
This function try to login into the Google account and obtain the authentication
code. The function will block until the request is finalized.
\param mail the full mail address.
\param password the password for the provided mail account.
\return true if the login was performed successfully.
*/
bool AuthService::login(const QString& mail, const QString& password)
{
  QUrl address("https://www.google.com/accounts/ClientLogin");

  QNetworkRequest request = QNetworkRequest();
  request.setUrl(address);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  QByteArray data;
  data += "accountType=HOSTED_OR_GOOGLE";
  data += "&Email="   + mail;
  data += "&Passwd="  + password;
  data += "&service=" + service;
  data += "&source="  + QCoreApplication::organizationName() + "-" + 
                        QCoreApplication::applicationName() + "-" + 
                        QCoreApplication::applicationVersion();

  reply = netAccMan->post(request, data);
  connect(reply, SIGNAL(finished()), this, SLOT(loginFinished()));

  loop->exec();

  return error.isEmpty();
}

/*!
Get the error text if it is the case.
*/
QString AuthService::getError()
{
  return error;
}

/*!
Get the authentication code.
*/
QString AuthService::getAuth()
{
  return auth;
}

QString AuthService::getLsid()
{
  return lsid;
}

QString AuthService::getSid()
{
  return sid;
}

/*!
This function handle the response of the google authentication server.
*/
void AuthService::loginFinished()
{
  if (!reply->error()) 
  {
    QList<QByteArray> response = reply->readAll().split('\n');
    error = "";
    for (int i = 0; i < response.size(); ++i) 
    {
      const QByteArray& responseToken = response.at(i);
      if (responseToken.startsWith("SID="))
        sid = responseToken.mid(4);
      else if (responseToken.startsWith("LSID="))
        lsid = responseToken.mid(5);
      else if (responseToken.startsWith("Auth="))
        auth = responseToken.mid(5);
      else if (responseToken.startsWith("Error="))
        error = responseToken.mid(6);
    }
  } 
  else 
  {
    error = reply->errorString();
    sid   = "";
    lsid  = "";
    auth  = "";
  }

  reply->deleteLater();
  reply = 0;
  loop->exit();
}
