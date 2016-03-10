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
#include "DaysTable.h"

/*!
Create a new instance of the DaysTable class.
*/
DaysTable::DaysTable(QWidget *parent) : QWidget(parent)
{
  createLayout();
  createAnimations();
}

/*!
Clean up.
*/
DaysTable::~DaysTable()
{
}

/*!
This function creates the layout and components of the widget.
*/
void DaysTable::createLayout()
{
  QVBoxLayout *mainLayout = new QVBoxLayout();
  QGridLayout *daysLayout = new QGridLayout();

  viewport = new QWidget();
  mainLayout->addWidget(viewport);

  for (int i = 1; i < 8; i++ )
  {
    QString weekDayText = QString("<b><font size=\"4\" color=\"black\" face=\"Verdana\">%1</font></color></b>").
                          arg(QDate::shortDayName(i));

    Token *label = new Token(this);
    label->setAlignment(Qt::AlignCenter);
    label->setText(weekDayText);
    daysLayout->addWidget(label, 0 , i);
  }

  for (int i = 1; i < 7; i++ )
  {
    Token *label = new Token(this);
    label->setAlignment(Qt::AlignCenter);
    cwList.append(label);
    daysLayout->addWidget(label, i , 0);
  }

  for (int i = 1; i < 7; i++ )
  {
    for (int j = 1; j < 8; j++ )
    {
      Token *label = new Token(this);
      label->setAlignment(Qt::AlignCenter);
      daysList.append(label);
      daysLayout->addWidget(label, i , j);
    }
  }

  daysLayout->setSpacing(0);
  daysLayout->setContentsMargins(0,0,0,0);
  daysLayout->setAlignment(Qt::AlignCenter);
  viewport->setLayout(daysLayout);

  mainLayout->setSpacing(0);
  mainLayout->setContentsMargins(0,0,0,0);
  mainLayout->setAlignment(Qt::AlignCenter);
  setLayout(mainLayout);
}

/*!
This function creates the required animations.
*/
void DaysTable::createAnimations()
{
  QPropertyAnimation *animation;

  animationGroup = new QSequentialAnimationGroup(this);

  animation = new QPropertyAnimation(viewport, "geometry");
  animation->setDuration(400);
  animation->setEasingCurve(QEasingCurve::InQuad);
  animationGroup->addAnimation(animation);

  animation = new QPropertyAnimation(viewport, "geometry");
  animation->setDuration(400);
  animation->setEasingCurve(QEasingCurve::OutQuad);
  animationGroup->addAnimation(animation);
}

/*!
This function performs a right to left slide animation.
*/
void DaysTable::animateLeft()
{
  QPropertyAnimation *animation;

  if (animationGroup->state() != QAbstractAnimation::Running)
  {
    animation = dynamic_cast<QPropertyAnimation *>(animationGroup->animationAt(0));
    animation->setStartValue(viewport->geometry());
    animation->setEndValue(viewport->geometry().adjusted(viewport->geometry().width(),0,viewport->geometry().width(),0));

    animation = dynamic_cast<QPropertyAnimation *>(animationGroup->animationAt(1));
    animation->setStartValue(viewport->geometry().adjusted(-viewport->geometry().width(),0,-viewport->geometry().width(),0));
    animation->setEndValue(viewport->geometry());

    animationGroup->start();
  }
}

/*!
This function performs a left to right slide animation.
*/
void DaysTable::animateRight()
{
  QPropertyAnimation *animation;

  if (animationGroup->state() != QAbstractAnimation::Running)
  {
    animation = dynamic_cast<QPropertyAnimation *>(animationGroup->animationAt(0));
    animation->setStartValue(viewport->geometry());
    animation->setEndValue(viewport->geometry().adjusted(-viewport->geometry().width(),0,-viewport->geometry().width(),0));

    animation = dynamic_cast<QPropertyAnimation *>(animationGroup->animationAt(1));
    animation->setStartValue(viewport->geometry().adjusted(viewport->geometry().width(),0,viewport->geometry().width(),0));
    animation->setEndValue(viewport->geometry());

    animationGroup->start();
  }
}

/*!
This function performs a top to bottom slide animation.
*/
void DaysTable::animateBottom()
{
  QPropertyAnimation *animation;

  if (animationGroup->state() != QAbstractAnimation::Running)
  {
    animation = dynamic_cast<QPropertyAnimation *>(animationGroup->animationAt(0));
    animation->setStartValue(viewport->geometry());
    animation->setEndValue(viewport->geometry().adjusted(0,-viewport->geometry().height(),0,-viewport->geometry().height()));

    animation = dynamic_cast<QPropertyAnimation *>(animationGroup->animationAt(1));
    animation->setStartValue(viewport->geometry().adjusted(0,viewport->geometry().height(),0,viewport->geometry().height()));
    animation->setEndValue(viewport->geometry());

    animationGroup->start();
  }
}

/*!
This function performs a bottom to top slide animation.
*/
void DaysTable::animateTop()
{
  QPropertyAnimation *animation;

  if (animationGroup->state() != QAbstractAnimation::Running)
  {
    animation = dynamic_cast<QPropertyAnimation *>(animationGroup->animationAt(0));
    animation->setStartValue(viewport->geometry());
    animation->setEndValue(viewport->geometry().adjusted(0,viewport->geometry().height(),0,viewport->geometry().height()));

    animation = dynamic_cast<QPropertyAnimation *>(animationGroup->animationAt(1));
    animation->setStartValue(viewport->geometry().adjusted(0,-viewport->geometry().height(),0,-viewport->geometry().height()));
    animation->setEndValue(viewport->geometry());

    animationGroup->start();
  }
}

/*!
This function displays the calendar for a given date.
\param date the date for which to show the calendar.
*/
void DaysTable::displayDate(QDate date)
{
  QString monthText;
  QString yearText;
  QDate   tempDate;
  QDate   firstDate;

  monthText = QString("<font size=\"5\" color=\"black\" face=\"Verdana\">%1</font></color>").
              arg(QDate::longMonthName(date.month()));

  yearText = QString("<font size=\"5\" color=\"black\" face=\"Verdana\">%1</font></color>").
             arg(date.year());

  firstDate.setDate(date.year(), date.month(), 1);
  firstDate = firstDate.addDays(-firstDate.dayOfWeek() + 1);

  displayWeekNumbers(firstDate);
  displayDays(date, firstDate);
}

/*!
This function displays the week numbers on the right.
\param firstDate the first date of the calendar.
*/
void DaysTable::displayWeekNumbers(QDate firstDate)
{
  for (int i = 1; i < 7; i++ )
  {
    QString cwText;

    cwText = QString("<b><font size=\"4\" color=\"black\" face=\"Verdana\">%1</font></color></b>").
             arg(firstDate.weekNumber());

    cwList[i-1]->setText(cwText);
    firstDate = firstDate.addDays(7);
  }
}

/*!
This function displays the calendar days for a given date.
\param date the date for which to show the calendar.
\param firstDate the first date of the calendar.
*/
void DaysTable::displayDays(QDate date, QDate firstDate)
{
  QDate tmpDate = firstDate;

  for (int i = 1; i < 7; i++ )
  {
    for (int j = 1; j < 8; j++ )
    {
      int index = (i-1) * 7 + (j-1);
      daysList[index]->setDate(tmpDate, date.month());
      tmpDate = tmpDate.addDays(1);
    }
  }

  emit requestEvents(QDateTime(firstDate), QDateTime(firstDate.addDays(42)));
}

/*!
This function updates the calendar with the events information.
\param events the events to be displayed.
*/
void DaysTable::displayEvent(EventItem eventItem)
{
  for (int i = 1; i < 7; i++)
  {
    for (int j = 1; j < 8; j++)
    {
      int index = (i - 1) * 7 + (j - 1);
      daysList[index]->setEvent(eventItem);
    }
  }
}
