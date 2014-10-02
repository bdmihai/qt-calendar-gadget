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

#ifndef DAYSTABLE_H
#define DAYSTABLE_H

#include "Token.h"

/*!
This class represents a table of tokens arranged in a calendar format.
*/
class DaysTable : public QWidget
{
  Q_OBJECT

  public:
    DaysTable(QWidget *parent = 0);
    virtual ~DaysTable();

    void createLayout();
    void createAnimations();

    void animateLeft();
    void animateRight();
    void animateBottom();
    void animateTop();

  signals:
    void requestEvents(const QDate &newStartDate, const QDate &newEndDate);

  public slots:
    void displayDate(QDate date);
    void displayEvents(EventList events);

  private:
    QWidget *viewport;
    QList<Token *> cwList;
    QList<Token *> daysList;
    QAnimationGroup *animationGroup;

    void displayWeekNumbers(QDate firstDate);
    void displayDays(QDate date, QDate firstDate);
};

#endif