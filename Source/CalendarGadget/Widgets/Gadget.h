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

#ifndef GADGET_H
#define GADGET_H

#include "Button.h"

/*!
This class represents a gadget. It has a few basic functionality implemented like
showing when the user shows the desktop, animations and a close button.
*/
class Gadget : public QWidget
{
  Q_OBJECT

  public:
    Gadget(QWidget *parent = 0);
    virtual ~Gadget();
    
    static void assureVisibility();
    void setLayout(QLayout *layout);

  protected slots:
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent * event);
    virtual void animate(int time, double desiredOpacity);
    virtual void closeGadget();
    virtual void aboutToHideMenu();

  protected:
    QPropertyAnimation *animation;
    QList<QWidget*> toolbox;
    QColor backColor;
    Button *closeButton;
    QPoint dragStartPoint;
    QSize  dragStartSize;
    bool   dragging;
    bool   resizing;
    bool   canClose;

  private:
    HWINEVENTHOOK globalHookId;
    static QWidgetList gadgetList;
    
  protected:
    void createLayout();
    void createAnimations();

    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void resizeEvent(QResizeEvent * event);
    virtual bool event(QEvent *event);

};

#endif