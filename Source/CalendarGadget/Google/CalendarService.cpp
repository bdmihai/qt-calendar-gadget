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
#include "CalendarService.h"

/*!
Create a new instance of the CalendarService class.
*/
CalendarService::CalendarService(QObject *parent, AuthService *auth) :
  QObject(parent),
  netAccMan(parent)
{
  this->auth = auth;

  connect(&netAccMan, SIGNAL(finished(QNetworkReply *)),
          this, SLOT(getCalendarsFinished(QNetworkReply *)));
  connect(this, SIGNAL(getNextCalendars(const QString &)),
          this, SLOT(getCalendars(const QString &)), Qt::QueuedConnection);
}

/*!
Clean up.
*/
CalendarService::~CalendarService()
{
}

/*!
Get the calendars from Google.
/param pageToken Next page token for handling more calendars.
*/
void CalendarService::getCalendars(const QString &pageToken)
{
  /*! This endpoint returns entries on the user's calendar list.*/
  const QString endpoint = "https://www.googleapis.com/calendar/v3/users/me/calendarList";

  /*! Maximum number of entries returned on one result page. By default the
  value is 100 entries. The page size can never be larger than 250 entries.
  Optional. (integer, 1+) */
  const QString maxResults = "200";

  /*! Selector specifying which fields to include in a partial response. */
  const QString fields = "items(description,id,summary),nextPageToken";

  // build up the query for url
  QUrlQuery urlQuery;
  urlQuery.addQueryItem("maxResults", maxResults);
  urlQuery.addQueryItem("fields", fields);
  if (pageToken != "")
    urlQuery.addQueryItem("pageToken", pageToken);

  // build up the url for the request
  QUrl url(endpoint);
  url.setQuery(urlQuery);

  // set-up the request
  QNetworkRequest request = QNetworkRequest();
  request.setUrl(url);
  request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer ") + auth->getAccessToken().toLatin1());

  // get calendars request
  netAccMan.get(request);
}

/*!
Parse the received data.
*/
void CalendarService::getCalendarsFinished(QNetworkReply *reply)
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
        CalendarItem calendarItem(value.toObject());
        emit calendarAvailable(calendarItem);
      }

      QJsonValue value = jsonObject["nextPageToken"];
      if (value.isString())
      {
        QString nextPageToken = value.toString();
        emit getNextCalendars(nextPageToken);
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
  reply = 0;
}
