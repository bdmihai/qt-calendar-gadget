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

#ifndef CALENDAR_H
#define CALENDAR_H

#include "Gadget.h"
#include "DaysTable.h"
#include "Button.h"
#include "DropDown.h"

#include "..\Google\GoogleService.h"
#include <QAbstractNativeEventFilter>

class Calendar;

class PowerbroadcastEventFilter : public QAbstractNativeEventFilter {
public:
    PowerbroadcastEventFilter();
    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) Q_DECL_OVERRIDE;
    void setCalendardReference(Calendar* calendar);
private:
    Calendar* calendar;
};

/*!
This represents the main window for the application.
*/
class Calendar : public Gadget
{
  Q_OBJECT

  public:
    Calendar(QWidget *parent = 0);
    virtual ~Calendar();
    boolean refreshGoogleService();

  protected:
    void createLayout();
    void connectDaysTableToGoogleService();
    virtual void showEvent(QShowEvent * event);
    virtual void closeEvent(QCloseEvent *event);

  public:
      void setCurrentDate(QDate date);
      void setCalculatedDay(QDate date);
      QDate getCalculatedDay();
      PowerbroadcastEventFilter* getPowerbroadcastEventFilter();
  
  public slots:
      void updateDisplay();

  protected slots:
    void updateCurrent();
    void updateYears();
    void updateMonths();
    void goToPrevMonth();
    void goToNextMonth();
    void goToYear();
    void goToMonth();
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
    QDate calculatedDay;

    GoogleService *googleService;
    PowerbroadcastEventFilter powerbroadcastEventFilter;
};

#endif