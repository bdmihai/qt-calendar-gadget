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

#ifndef EVENT_H
#define EVENT_H

/*!
This class represent a single all-day-event.
*/
class Event : public QObject
{
  public:
    Event();
    Event(const Event &other);
    virtual ~Event();

    QString getTitle();
    void setTitle(const QString &newTitle);
    QDate getDate();
    void setDate(const QDate &newDate);

    Event & operator = (const Event &other);

  private:
    QDate date;
    QString title;
};

//! A type definition for a list of events
typedef QList<Event> EventList;

#endif
