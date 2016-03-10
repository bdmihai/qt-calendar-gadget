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

#ifndef GOOGLESERVICE_H
#define GOOGLESERVICE_H

#include "AuthService.h"
#include "CalendarService.h"
#include "EventService.h"

/*!
This class represents the calendar service necessary for accessing the
data from the google calendar. The necessary documentation for using this
service can be found at Google: https://developers.google.com/google-apps/calendar/
*/
class GoogleService : public QObject
{
    Q_OBJECT

  public:
    GoogleService(QObject *parent);
    virtual ~GoogleService();

    bool isServiceAvailable();
    QUrl getAuthRequestUrl();
    QString getLoginError();
    QStringList getErrors();

  signals:
    void eventAvailable(EventItem eventItem);

  public slots:
    bool login(const QString &code);
    void getEvents(const QDateTime &start, const QDateTime &end);

  private slots:
    void onCalendarAvailable(CalendarItem calendarItem);
    void onEventAvailable(EventItem eventItem);
    void onError(const QString &error);

  private:
    AuthService authService;
    CalendarService calendarService;
    EventService eventService;
    QDateTime start;
    QDateTime end;
    QStringList errors;
};

#endif // GOOGLESERVICE_H
