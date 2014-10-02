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

#ifndef CALENDARSERVICE_H
#define CALENDARSERVICE_H

#include "Event.h"
#include "AuthService.h"

/*!
This class represents the calendar service necessary for accessing the
events from the google calendar. The necessary documentation for using this 
service can be found at Google:
http://code.google.com/intl/en/apis/calendar/data/2.0/reference.html
*/
class CalendarService : public QObject
{
  Q_OBJECT

  public:
    CalendarService();
    virtual ~CalendarService();

    bool isServiceAvailable();
    QString getError();

    bool login(QString mail, QString password);
    void configureProxy(QNetworkProxy::ProxyType type, const QString &hostName = QString(), quint16 port = 0, const QString &user = QString(), const QString &password = QString());

  signals:
    void eventsAvailable(EventList events);

  public slots:
    void getEvents(const QDate &newStartDate, const QDate &newEndDate);

  protected slots:
    virtual void getSessionDone();
    virtual void getEventsDone();

  protected:
    bool serviceWorking;
    QString error;
    QString session;
    QNetworkAccessManager *netAccMan;
    QNetworkReply *reply;
    QEventLoop *loop;
    AuthService *auth;

    bool getSession();
};

#endif