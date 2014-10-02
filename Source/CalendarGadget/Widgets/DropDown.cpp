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
#include "DropDown.h"

/*!
Create a new instance of the DropDown class.
*/
DropDown::DropDown(QWidget *parent) : Button(parent)
{
  dropDownPopup = new DropDownPopup(this);
  connect(dropDownPopup, SIGNAL(aboutToHide()), this, SIGNAL(aboutToHide()));
}

/*!
Clean up.
*/
DropDown::~DropDown()
{
}

/*!
Add a button to the popup.
*/
Button *DropDown::addButton(const QString &text)
{
  return dropDownPopup->addButton(text);
}

/*!
Remove all buttons from the popup.
*/
void DropDown::clear()
{
  dropDownPopup->clear();
}

/*!
Override the mouse release event. 
Emits the aboutToShow signal if the release event happens in the widget's rect.
Shows the popup if the release event happens in the widget's rect. 
Emits the clicked signal if the release event happens in the widget's rect. 
*/
void DropDown::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    if (rect().contains(event->pos()))
    {
      emit aboutToShow();
      dropDownPopup->popup(event->globalPos());
      emit clicked();
    }
  }
}
