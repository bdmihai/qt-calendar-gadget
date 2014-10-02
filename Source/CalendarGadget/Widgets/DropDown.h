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

#ifndef DROPDOWN_H
#define DROPDOWN_H

#include "Button.h"
#include "DropDownPopup.h"

/*!
This represents a drop down button.
*/
class DropDown : public Button
{
  Q_OBJECT

  public:
    DropDown(QWidget *parent = 0);
    virtual ~DropDown();

    Button *addButton(const QString &text);
    void clear();

  signals:
    void aboutToHide();
    void aboutToShow();

  protected:
    virtual void mouseReleaseEvent(QMouseEvent * event);

  private:
    DropDownPopup *dropDownPopup;
};

#endif // DROPDOWN_H
