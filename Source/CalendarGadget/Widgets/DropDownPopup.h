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

#ifndef DROPDOWNPOPUP_H
#define DROPDOWNPOPUP_H

#include "Button.h"

/*!
This represents the popup menu associated with a drop down button.
*/
class DropDownPopup : public QWidget
{
  Q_OBJECT

  public:
    DropDownPopup(QWidget *parent = 0);
    virtual ~DropDownPopup();

    Button *addButton(const QString &text);
    void clear();
    void popup(const QPoint &popupLocation);

  signals:
    void aboutToShow();
    void aboutToHide();

  protected slots:
    void buttonClicked();
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent * event);
    virtual void paintEvent(QPaintEvent *event);

  private:
    QList<Button *> buttonList;
};

#endif // DROPDOWNPOPUP_H
