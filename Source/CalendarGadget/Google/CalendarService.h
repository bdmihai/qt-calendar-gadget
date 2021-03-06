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

#ifndef CALENDARSERVICE_H
#define CALENDARSERVICE_H

#include "AuthService.h"
#include "CalendarItem.h"

/*!
This class represents the calendar service necessary for accessing the
calendars from the google calendar. The necessary documentation for using this
service can be found at Google: https://developers.google.com/google-apps/calendar/
*/
class CalendarService : public QObject
{
    Q_OBJECT

  public:
    CalendarService(QObject *parent, AuthService *auth);
    virtual ~CalendarService();

  signals:
    void error(const QString &error);
    void calendarAvailable(CalendarItem calendarItem);
    void getNextCalendars(const QString &pageToken);

  public slots:
    void getCalendars(const QString &pageToken = "");

  protected slots:
    void getCalendarsFinished(QNetworkReply *reply);

  protected:
    QNetworkAccessManager netAccMan;
    AuthService *auth;
};

#endif