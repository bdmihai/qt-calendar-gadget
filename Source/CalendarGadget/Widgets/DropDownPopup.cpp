/****************************************************************************
**
** Copyright (C) 2010-2012 B.D. Mihai.
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
#include "DropDownPopup.h"

/*!
Create a new instance of the DropDownPopup class.
*/
DropDownPopup::DropDownPopup(QWidget *parent) : QWidget(parent)
{
  QVBoxLayout *vLayout= new QVBoxLayout();

  vLayout->setContentsMargins(10, 7, 10, 7);
  vLayout->setSpacing(2);
  setLayout(vLayout);
  setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
}

/*!
Clean up.
*/
DropDownPopup::~DropDownPopup()
{
  qDeleteAll(buttonList);
  buttonList.clear();
}

/*!
Add a button to the popup.
*/
Button *DropDownPopup::addButton(const QString &text)
{
  Button *button = new Button();
  button->setText("<font size=\"4\" color=\"black\" face=\"Verdana\">" + text + "</font></color>");
  button->setAlignment(Qt::AlignCenter);
  button->setTag(text);
  button->setOptimumSize(0, 25);
  buttonList.append(button);
  layout()->addWidget(button);
  connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
  return button;
}

/*!
Remove all buttons from the popup.
*/
void DropDownPopup::clear()
{
  foreach (Button *button, buttonList)
  {
    layout()->removeWidget(button);
  }
  qDeleteAll(buttonList);
  buttonList.clear();
}

/*!
Handle the button clicked event and hide the widget.
*/
void DropDownPopup::buttonClicked()
{
  hide();
}

/*!
Handle the widget close event and emit a signal that the widget is bout to be hidden.
*/
void DropDownPopup::closeEvent(QCloseEvent *event)
{
  emit aboutToHide();
}

/*!
Handle the widget show event and emit a signal that the widget is bout to be showed.
*/
void DropDownPopup::showEvent(QShowEvent * event)
{
  emit aboutToShow();
}

/*!
Show the popup widget at the provided location.
\popupLocation global point location wher to show the popup widget.
*/
void DropDownPopup::popup(const QPoint &popupLocation)
{
  show();
  move(popupLocation);
}

/*!
Override the paint event in order to fill the background display the pixmap and 
call the QWidget paint event to draw the text.
*/
void DropDownPopup::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  QBrush brush;
  QPen pen;
  QRect drawRect;

  pen = QPen(Qt::gray, 1);
  brush = QBrush(QColor::fromRgb (200, 200, 200));

  painter.setRenderHint(QPainter::Antialiasing, false);
  painter.setPen(pen);
  painter.setBrush(brush);
  drawRect = QRect(rect().x(), rect().y(), rect().width()-1, rect().height()-1);
  painter.drawRoundedRect(drawRect, 2, 2, Qt::AbsoluteSize);

  QWidget::paintEvent(event);
}
