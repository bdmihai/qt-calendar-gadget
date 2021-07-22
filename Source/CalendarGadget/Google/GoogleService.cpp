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
#include "GoogleService.h"

/*!
Create a new instance of the GoogleService class.
*/
GoogleService::GoogleService(QObject *parent) :
  QObject(parent),
  authService(this),
  calendarService(this, &authService),
  eventService(this, &authService)
{
  connect(&calendarService, SIGNAL(error(const QString &)),
          this, SLOT(onError(const QString &)));

  connect(&eventService, SIGNAL(error(const QString &)),
          this, SLOT(onError(const QString &)));

  connect(&calendarService, SIGNAL(calendarAvailable(CalendarItem)),
          this, SLOT(onCalendarAvailable(CalendarItem)));

  connect(&eventService, SIGNAL(eventAvailable(EventItem)),
          this, SLOT(onEventAvailable(EventItem)));
}

/*!
Clean up.
*/
GoogleService::~GoogleService()
{
}

/*!
Check if the service is available.
*/
bool GoogleService::isServiceAvailable()
{
  return authService.getAccessToken() != "";
}

/*!
Get the authentication url required for user login.
*/
QUrl GoogleService::getAuthRequestUrl()
{
  return authService.getAuthRequestUrl();
}

/*!
Get the authentication error.
*/
QString GoogleService::getLoginError()
{
  return authService.getError();
}

/*!
Get list of Google service errors.
*/
QStringList GoogleService::getErrors()
{
  return errors;
}

/*!
Login using the authentication code received after the user granted access.
*/
bool GoogleService::login(const QString &code)
{
  return authService.exchangeCodeForToken(code);
}

/*!
Get the events.
*/
void GoogleService::getEvents(const QDateTime &start, const QDateTime &end)
{
  this->start = start;
  this->end = end;

  if (isServiceAvailable())
  {
    calendarService.getCalendars();
  }
  else
  {
    if (authService.hasRefreshToken())
    {
      if (authService.refreshToken())
      {
        calendarService.getCalendars();
      }
    }
  }
}

/*!
Event handler for calendar available.
*/
void GoogleService::onCalendarAvailable(CalendarItem calendarItem)
{
  eventService.getEvents(calendarItem.id, start.toString(Qt::ISODate) + "Z", end.toString(Qt::ISODate) + "Z");
}

/*!
Event handler for event available.
*/
void GoogleService::onEventAvailable(EventItem eventItem)
{
  emit eventAvailable(eventItem);
}

/*!
Event handler for error.
*/
void GoogleService::onError(const QString &error)
{
  errors.append(error);
  qDebug().noquote() << "Google service error: " << error;
}
