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

#ifndef EVENTSERVICE_H
#define EVENTSERVICE_H

#include "AuthService.h"
#include "EventItem.h"

/*!
This class represents the calendar service necessary for accessing the
events from the google calendar. The necessary documentation for using this
service can be found at Google: https://developers.google.com/google-apps/calendar/
*/
class EventService : public QObject
{
    Q_OBJECT

  public:
    EventService(QObject *parent, AuthService *auth);
    virtual ~EventService();

  signals:
    void error(const QString &error);
    void eventAvailable(EventItem eventItems);
    void getNextEvents(const QString &calendarId, const QString &start, const QString &end, const QString &pageToken);

  public slots:
    void getEvents(const QString &calendarId, const QString &start, const QString &end, const QString &pageToken = "");

  protected slots:
    void getEventsFinished(QNetworkReply *reply);

  protected:
    QNetworkAccessManager netAccMan;
    AuthService *auth;
};

#endif // EVENTSERVICE_H
