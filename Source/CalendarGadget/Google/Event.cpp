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
#include "Event.h"

/*!
Create a new instance of the Event class.
*/
Event::Event()
{
  title = "";
  date = QDate::currentDate();
}

/*!
Create a new instance of the Event class using another Event.
*/
Event::Event(const Event &other)
{
  (*this) = other;
}

/*!
Clean up.
*/
Event::~Event()
{
}

/*!
Get the event title.
*/
QString Event::getTitle()
{
  return title;
}

/*!
Set the event title.
*/
void Event::setTitle(const QString &newTitle)
{
  title = newTitle;
}

/*!
Get the event occurrence date.
*/
QDate Event::getDate()
{
  return date;
}

/*!
Set the event occurrence date.
*/
void Event::setDate(const QDate &newDate)
{
  date = newDate;
}

/*!
Copy operator.
*/
Event & Event::operator = (const Event &other)
{
  this->title = other.title;
  this->date = other.date;

  return *this;
}
