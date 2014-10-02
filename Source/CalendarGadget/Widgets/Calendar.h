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

#ifndef CALENDAR_H
#define CALENDAR_H

#include "Gadget.h"
#include "DaysTable.h"
#include "Button.h"
#include "DropDown.h"

#include "..\Google\CalendarService.h"

/*!
This represents the main window for the application.
*/
class Calendar : public Gadget
{
  Q_OBJECT

  public:
    Calendar(QWidget *parent = 0);
    virtual ~Calendar();

  protected:
    void createLayout();
    virtual void closeEvent(QCloseEvent *event);

  protected slots:
    void updateCurrent();
    void updateDisplay();
    void updateMonths();
    void updateYears();
    void goToPrevMonth();
    void goToNextMonth();
    void goToMonth();
    void goToYear();
    void goToToday();
    void showConnect();
    void showAbout();

  private:
    DaysTable *daysTable;
    Button    *todayButton;
    Button    *nextMonth;
    Button    *prevMonth;
    DropDown  *currentMonth;
    DropDown  *currentYear;
    Button    *connectButton;
    Button    *aboutButton;

    QTimer updateTimer;
    QDate  currentDate;
    QTime  currentTime;
    QDate  displayDate;

    CalendarService *calendarService;
};

#endif