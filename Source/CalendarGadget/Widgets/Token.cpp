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
#include "DaysTable.h"
#pragma warning(push)
#pragma warning(disable : 26409) 
// don't know how to do this to avoid warning C26409 
//      auto plus100DaysAction = std::make_unique<QAction>(tr("Add &100 Days"), this);  
// works but is not compatible with connect(), ErrorE0304	- no instance of overloaded function "Token::connect" matches the argument list
//           plus100DaysAction = std::make_unique<QAction>(tr("Add &100 Days"), this);  
// results in E0413	- no suitable conversion function from "std::unique_ptr<QAction, std::default_delete<QAction>>" to "QAction *" exists	


/*!
Create a new instance of the Token class.
*/
Token::Token(QWidget* parent) : QLabel(parent)
{
	date = QDate();
	daysTable = parent;
    if (dynamic_cast<DaysTable*>(daysTable) == nullptr) exit(99);

	setAttribute(Qt::WA_Hover, true);
	resize(200, 200);

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// create items for context menu 
	dateTitle = new QLabel("Today");
	dateTitle->setAlignment(Qt::AlignCenter);
	setDateTitleFontBold();
	dateTitleAction = new QWidgetAction(this);
	dateTitleAction->setDefaultWidget(dateTitle);

	plus100DaysAction = new QAction(tr("Add 100 &days"), this);
	plus100DaysAction->setShortcut(QKeySequence(Qt::Key_D));  // TODO: shortcut key 'D' does not work - no idea why, the others do
	plus100DaysAction->setStatusTip(tr("Add 100 days to selected date"));
	connect(plus100DaysAction, &QAction::triggered, this, &Token::plus100Days);

	plusOneMonthAction = new QAction(tr("Add &one month"), this);
	plusOneMonthAction->setShortcut(QKeySequence(Qt::Key_O));
	plusOneMonthAction->setStatusTip(tr("Add one full month to selected date"));
	connect(plusOneMonthAction, &QAction::triggered, this, &Token::plusOneMonth);

	plus4WeeksAction = new QAction(tr("Add &four weeks"), this);
	plus4WeeksAction->setShortcut(QKeySequence(Qt::Key_F));
	plus4WeeksAction->setStatusTip(tr("Add four weeks to selected date"));
	connect(plus4WeeksAction, &QAction::triggered, this, &Token::plus4Weeks);

	daysTitle = new QLabel("Enter number of days:");
	daysTitle->setAlignment(Qt::AlignLeft);
	daysTitleAction = new QWidgetAction(this);
	daysTitleAction->setDefaultWidget(daysTitle);

	days = new QLineEdit();
	QString numbers = "#######";
	days->setMaxLength(numbers.length());
	days->setInputMask(numbers);
	days->setPlaceholderText("200");
	daysAction = new QWidgetAction(this);
    daysAction->setDefaultWidget(days);

	bool connected = connect(days, &QLineEdit::returnPressed, this, &Token::processDays);
	Q_ASSERT(connected);

}

#pragma warning(pop)

void Token::setDateTitleFontBold()
{
    QFont boldFont = dateTitle->font();
    boldFont.setBold(true);
    dateTitle->setFont(boldFont);
}

/*!
Clean up.
*/
Token::~Token()
{
    delete dateTitle;
    delete dateTitleAction;
    delete plus100DaysAction;
    delete plusOneMonthAction;
    delete plus4WeeksAction;
    delete daysTitle;
    delete daysTitleAction;
    delete days;
    delete daysAction;
}


/*!
This function sets the date of the day for the token and updates the context menu accordingly
*/
void Token::setDate(const QDate &newDate, const int &currentMonth)
{
  date = newDate;
  month = currentMonth;
  setDisplayText(false);
  setToolTip("");
  updateContextMenu();
}

void Token::updateContextMenu()
{
    QString label = "Add 100 days to " + this->date.toString(Qt::SystemLocaleDate);
    plus100DaysAction->setText(label);

    setDateTitleFontBold();
    dateTitle->setText(this->date.toString(Qt::SystemLocaleDate));
}

/*!
This function sets the event for the token. Each event is passed and the token
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
Format the text that shall be displayed taking into account the availability of 
an all day event.
*/
void Token::setDisplayText(bool hasAllDayEvent)
{
	QString dayText;
	QString maroon = "<font size=\"4\" color=\"maroon\" face=\"Verdana\">%1</font></color>";
	QString boldMaroon = "<b>" + maroon + "< / b>";
	QString boldGreen = "<b><font size = \"5\" color=\"green\" face=\"Verdana\">%1</font></color></b>";
	QString black = "<font size=\"4\" color=\"black\" face=\"Verdana\">%1</font></color>";
	QString boldBlack = "<b>" + black + "< / b>";
    QString gray = "<font size=\"4\" color=\"gray\" face=\"Verdana\">%1</font></color>";

	if (hasAllDayEvent)
	{
		if (date.month() == month)
		{
			if (date == QDate::currentDate())
			{
				dayText = QString(boldMaroon).arg(date.day());
			}
			else if (date == ((DaysTable*)daysTable)->getCalculatedDay())
			{
				dayText = QString(boldGreen).arg(date.day());
			}
			else
			{
				dayText = QString(boldMaroon).arg(date.day());
			}
		}
		else
		{
			dayText = QString(maroon).arg(date.day());
		}
	}
	else
	{
		if (date.month() == month)
		{
			if (date == QDate::currentDate())
			{
				dayText = QString(boldBlack).arg(date.day());
			}
			else if (date == ((DaysTable*)daysTable)->getCalculatedDay())
			{
				dayText = QString(boldGreen).arg(date.day());
			}
			else
			{
				dayText = QString(black).arg(date.day());
			}
		}
		else
		{
			if (date == ((DaysTable*)daysTable)->getCalculatedDay())
			{
				dayText = QString(boldGreen).arg(date.day());
			}
			else
			{
				dayText = QString(gray).arg(date.day());
			}
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

void Token::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    menu.addAction(dateTitleAction);
    menu.addSeparator();
    menu.addAction(plus100DaysAction);
    menu.addAction(plusOneMonthAction);
    menu.addAction(plus4WeeksAction);
    menu.addSeparator();
    menu.addAction(daysTitleAction);
    menu.addAction(daysAction);
    menu.setActiveAction(menu.actions()[menu.actions().indexOf(daysAction)]);
    menu.exec(event->globalPos());
}

void Token::plus100Days()
{
    const QDate calculatedDay = this->date.addDays(100);
    qDebug().noquote() << this->date.toString(Qt::SystemLocaleDate) + " + 100 days = " + calculatedDay.toString(Qt::SystemLocaleDate);
    skipForwardAndHilite(calculatedDay, calculatedDay.toString(Qt::SystemLocaleDate) + " is 100 days after " + this->date.toString(Qt::SystemLocaleDate));
}

void Token::plusOneMonth()
{
    const QDate calculatedDay = this->date.addMonths(1);
    qDebug().noquote() << this->date.toString(Qt::SystemLocaleDate) + " + one month = " + calculatedDay.toString(Qt::SystemLocaleDate);
    skipForwardAndHilite(calculatedDay, calculatedDay.toString(Qt::SystemLocaleDate) + " is one month after the " + this->date.toString(Qt::SystemLocaleDate));
}

void Token::plus4Weeks()
{
    const QDate calculatedDay = this->date.addDays(4 * 7);
    qDebug().noquote() << this->date.toString(Qt::SystemLocaleDate) + " + 4 weeks = " + calculatedDay.toString(Qt::SystemLocaleDate);
    skipForwardAndHilite(calculatedDay, calculatedDay.toString(Qt::SystemLocaleDate) + " is four weeks after " + this->date.toString(Qt::SystemLocaleDate));
}

void Token::processDays()
{
    const QDate calculatedDay = this->date.addDays(days->text().toInt());
    qDebug().noquote() << this->date.toString(Qt::SystemLocaleDate) + " +" + days->text() + " days = " + calculatedDay.toString(Qt::SystemLocaleDate);
    skipForwardAndHilite(calculatedDay, calculatedDay.toString(Qt::SystemLocaleDate) + " is " + days->text() + " days from " + this->date.toString(Qt::SystemLocaleDate));
    daysAction->trigger();
}

void Token::skipForwardAndHilite(const QDate& calculatedDay, const QString& DescriptionOfTheOperationPerformed) {
    if (calculatedDay.month() > month) {
        ((DaysTable*)daysTable)->animateRight();
    }
    else if (calculatedDay.month() < month) {
        ((DaysTable*)daysTable)->animateLeft();
    }
    ((DaysTable*)daysTable)->setCalculatedDay(calculatedDay);
    ((DaysTable*)daysTable)->displayDate(calculatedDay);

    const QJsonObject event = QJsonObject(
        {
            // field definitions see https://fullcalendar.io/docs/events-json-feed
            {"id", "calculated day"},
            {"summary", DescriptionOfTheOperationPerformed},
            {"start", QJsonObject { {"date", calculatedDay.toString("yyyy-MM-dd")}}},
            {"end", QJsonObject { {"date", calculatedDay.toString("yyyy-MM-dd")}}}
        });
    EventItem eventItem(event);
    ((DaysTable*)daysTable)->displayEvent(eventItem);

    updateContextMenu();
}


/*!
Override the paint event. If the token displays a date and this is the current
date this functions also draws the background, looking like a gray button
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
