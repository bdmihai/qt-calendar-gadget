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
#include "EventItem.h"

EventItem::EventItem(const QJsonObject &jsonObject)
{
  id = jsonObject["id"].toString();
  summary = jsonObject["summary"].toString();

  if (jsonObject["start"].toObject().contains("date"))
  {
    start = jsonObject["start"].toObject().value("date").toVariant().toDateTime();
    allDayEvent = true;
  }
  else
  {
    allDayEvent = false;
  }

  if (jsonObject["start"].toObject().contains("dateTime"))
    start = jsonObject["start"].toObject().value("dateTime").toVariant().toDateTime();

  if (jsonObject["end"].toObject().contains("date"))
    end = jsonObject["end"].toObject().value("date").toVariant().toDateTime();

  if (jsonObject["end"].toObject().contains("dateTime"))
    end = jsonObject["end"].toObject().value("dateTime").toVariant().toDateTime();
}

EventItem::~EventItem()
{
}
