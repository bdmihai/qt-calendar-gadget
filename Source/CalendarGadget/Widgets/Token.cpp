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
#include "Token.h"

/*!
Create a new instance of the Token class.
*/
Token::Token(QWidget *parent) : QLabel(parent)
{
  date = QDate();

  setAttribute(Qt::WA_Hover, true);
  resize(200,200);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

/*!
Clean up.
*/
Token::~Token()
{

}

/*!
This function sets the date for the token.
*/
void Token::setDate(const QDate &newDate, const int &currentMonth)
{
  date = newDate;
  month = currentMonth;
  setDisplayText(false);
  setToolTip("");
}

/*!
This function sets the event for the token. Each events is passed and the token
selects the relevant ones.
*/
void Token::setEvent(EventItem eventItem)
{
  if (date == eventItem.start.date())
  {
    QString title;
    if (eventItem.allDayEvent)
      title = "Today:";
    else
      title = QString("%1 - %2:").arg(eventItem.start.toString("hh:mm")).arg(eventItem.end.toString("hh:mm"));
    if (toolTip() == "")
    {
      QString tooltip = QString("<font size=\"3\" color=\"black\" face=\"Verdana\"><b>%1</b></font>"
                                "<ul><li><font size=\"3\" color=\"black\" face=\"Verdana\">%2</font></color></li></ul>").
                        arg(title).
                        arg(eventItem.summary);
      setToolTip(tooltip);
    }
    else
    {
      QString tooltip = QString("<font size=\"3\" color=\"black\" face=\"Verdana\"><b>%1</b></font>"
                                "<ul><li><font size=\"3\" color=\"black\" face=\"Verdana\">%2</font></color></li></ul>").
                        arg(title).
                        arg(eventItem.summary);
      setToolTip(toolTip() + tooltip);
    }


    if (eventItem.allDayEvent)
      setDisplayText(true);
  }
}

/*!
Format the test that shall be displayed taking into account the availability of 
a all day event.
*/
void Token::setDisplayText(bool hasAllDayEvent)
{
  QString dayText;

  if (hasAllDayEvent)
  {
    if (date.month() == month)
    {
      if (date == QDate::currentDate())
      {
        dayText = QString("<b><font size=\"4\" color=\"maroon\" face=\"Verdana\">%1</font></color></b>").
                  arg(date.day());
      }
      else
      {
        dayText = QString("<b><font size=\"4\" color=\"maroon\" face=\"Verdana\">%1</font></color></b>").
                  arg(date.day());
      }
    }
    else
    {
      dayText = QString("<font size=\"4\" color=\"maroon\" face=\"Verdana\">%1</font></color>").
                arg(date.day());
    }
  }
  else
  {
    if (date.month() == month)
    {
      if (date == QDate::currentDate())
      {
        dayText = QString("<b><font size=\"4\" color=\"black\" face=\"Verdana\">%1</font></color></b>").
                  arg(date.day());
      }
      else
      {
        dayText = QString("<font size=\"4\" color=\"black\" face=\"Verdana\">%1</font></color>").
                  arg(date.day());
      }
    }
    else
    {
      dayText = QString("<font size=\"4\" color=\"gray\" face=\"Verdana\">%1</font></color>").
                arg(date.day());
    }
  }

  setText(dayText);
}

/*!
Override the mouse move event. Call the default widget implementation in order
to pass this message to the parent also.
*/
void Token::mouseMoveEvent(QMouseEvent *event)
{
  QLabel::mouseMoveEvent(event);
  QWidget::mouseMoveEvent(event);
}

/*!
Override the mouse press event. Call the default widget implementation in order
to pass this message to the parent also.
*/
void Token::mousePressEvent(QMouseEvent *event)
{
  QLabel::mousePressEvent(event);
  QWidget::mousePressEvent(event);
}

/*!
Override the mouse release event. Call the default widget implementation in order
to pass this message to the parent also.
*/
void Token::mouseReleaseEvent(QMouseEvent *event)
{
  QLabel::mouseReleaseEvent(event);
  QWidget::mouseReleaseEvent(event);
}

/*!
Override the paint event. If the token displays a date and this is the current
date this functions also draws the background.
*/
void Token::paintEvent(QPaintEvent *event)
{
  if (date.isValid())
  {
    if (date == QDate::currentDate())
    {
      QPainter painter(this);
      QLinearGradient linearGradient;
      QRect drawRect;
      QPen pen;

      linearGradient = QLinearGradient(0, 0, 0, rect().height() + 1);
      linearGradient.setColorAt(0.0, QColor::fromRgb(255, 255, 180, 255));
      linearGradient.setColorAt(1.0, Qt::gray);

      pen = QPen(Qt::gray, 1);

      painter.setRenderHint(QPainter::Antialiasing, false);
      painter.setBrush(linearGradient);
      painter.setPen(pen);
      drawRect = QRect(rect().x(), rect().y(), rect().width()-1, rect().height()-1);
      painter.drawRoundedRect(drawRect, 2, 2, Qt::AbsoluteSize);
    }
  }

  QLabel::paintEvent(event);
}
