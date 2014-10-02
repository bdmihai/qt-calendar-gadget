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

#include "StdAfx.h"
#include "Gadget.h"

#define GADGET_CLIP_EDGE 20
#define GADGET_EDGE       5
#define GADGET_TOOL_EDGE 26

//! a global list with all the showed gadgets - used to assure visibility at "Show desktop"
QWidgetList Gadget::gadgetList;

/*!
This function is a windows handle registered to be called when the user wants
to show the desktop using the "Show desktop" function.
*/
void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, 
                             LONG idObject, LONG idChild, DWORD dwEventThread, 
                             DWORD dwmsEventTime) 
{
  if (event == EVENT_SYSTEM_FOREGROUND) 
  {
    TCHAR tmp[255];
    int r = GetClassName(hwnd, tmp, 250);
    if (wcscmp(tmp, L"WorkerW")==0) 
    {
      Gadget::assureVisibility();
    }
  }
}

/*!
Create a new instance of the Gadget class.
*/
Gadget::Gadget(QWidget *parent) : QWidget(parent)
{
  // global hook id - will be set after the window is showed
  globalHookId = 0;

  // internal state of the gadget
  canClose = false;
  dragging = false;
  resizing = false;

  // create the layout
  createLayout();

  // create the animations
  createAnimations();
}

/*!
Clean up.
*/
Gadget::~Gadget()
{
  UnhookWinEvent(globalHookId);
  Gadget::gadgetList.removeAll(this);
}

/*!
Create the gadget basic layout.
*/
void Gadget::createLayout()
{
  // create the close button
  closeButton = new Button(this);
  closeButton->setToolTip(tr("Close"));
  closeButton->setPixmap(QPixmap(":/Close.png"));
  closeButton->setColors(Qt::transparent, Qt::transparent, 
    QColor::fromRgb(255, 100, 100, 255), QColor::fromRgb(200, 0, 0, 255),
    QColor::fromRgb(255, 130, 130, 255), QColor::fromRgb(180, 0, 0, 255));
  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
  toolbox.append(closeButton);

  QWidget::setWindowFlags(Qt::SplashScreen | Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground, true);
  setAttribute(Qt::WA_Hover, true);
  setWindowOpacity(0.0);
  resize(200, 200);

  backColor = QColor::fromRgb(255, 255, 180, 255);
}

/*!
Create the gadget animations.
*/
void Gadget::createAnimations()
{
  animation = new QPropertyAnimation(this, "windowOpacity", this);
}

/*!
Animate the gadget. This function performs a simple animation of the opacity 
property. The transition starts from the current window opacity.
\param time the time for the  animation.
\param desiredOpacity the desired value for the opacity.
*/
void Gadget::animate(int time, double desiredOpacity)
{
  animation->setDuration(time);
  animation->setStartValue(windowOpacity());
  animation->setEndValue(desiredOpacity);
  animation->start();
}

/*!
This function assures that all gadgets created are visible on the desktop.
*/
void Gadget::assureVisibility()
{
  for (int k = 0; k < gadgetList.count(); k++)
  {
    WId handle = gadgetList[k]->winId();
    if (handle != 0)
    {
      // usually works on first try, but sometimes needs 2 or 3 
      for (int i = 0; i < 10; ++i)
      { 
        SetWindowPos((HWND)handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        SetWindowPos((HWND)handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
      }
    }
  }
}

/*!
Set the gadget layout. The edgeds are taken into consideration.
*/
void Gadget::setLayout(QLayout *layout)
{
  layout->setContentsMargins(GADGET_EDGE + 8, GADGET_EDGE + 8, GADGET_TOOL_EDGE + 8, GADGET_EDGE + 8);
  QWidget::setLayout(layout);
}

/*!
This function sets the state of the gadget to "canClose" and calls the widgets 
close().
*/
void Gadget::closeGadget()
{
  canClose = true;
  close();
}

/*!
This function animates the the gadget if the mouse cursor is outside the widget's
rectangle. This is meant to be connected at the aboutToHide signal for the context
menus because the widget will not receive anymore the leave enter events.
*/
void Gadget::aboutToHideMenu()
{
  QPoint globalPosition = QCursor::pos();
  QPoint localPosition = mapFromGlobal(globalPosition);
  if (!rect().contains(localPosition))
  {
    animate(300, 0.1);
  }
}

/*!
This function overrides the normal widget close event in order to perform the 
animation first.
*/
void Gadget::closeEvent(QCloseEvent *event)
{
  if (canClose)
  {
    return;
  }
  else
  {
    connect(animation, SIGNAL(finished()),
      this, SLOT(closeGadget()));
    animate(300, 0.0);
    event->ignore();
  }
}

/*!
This function overrides the normal widget show event in order to perform the 
animation and register the callback function.
*/
void Gadget::showEvent(QShowEvent * event)
{
  Gadget::gadgetList.append(this);
  globalHookId = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, (WINEVENTPROC)HandleWinEvent,0,0,0);
  animate(300, 0.1);
}

/*!
This function overrides the normal widget paint event in order to perform the 
drawing of the background.
*/
void Gadget::paintEvent(QPaintEvent * /* event */)
{
  QPainter painter(this);
  QLinearGradient linearGradient;
  QRect drawRect;
  
  linearGradient = QLinearGradient(0, 0, 0, rect().height()/3);
  linearGradient.setColorAt(0.0, QColor::fromRgb ( 200, 200, 200, 120 ));
  linearGradient.setColorAt(0.3, QColor::fromRgb ( 255, 255, 255, 120 ));
  linearGradient.setColorAt(1.0, QColor::fromRgb ( 60, 60, 60, 60 ));

  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(QPen(Qt::lightGray, 2));
  painter.setBrush(QBrush(Qt::transparent));
  drawRect = QRect(rect().x()+1, rect().y()+1, rect().width()-2, rect().height()-2);
  painter.drawRoundedRect(drawRect, 8, 8, Qt::AbsoluteSize);

  painter.setRenderHint(QPainter::Antialiasing, false);
  painter.setPen(QPen(Qt::black, 1));
  painter.setBrush(linearGradient);
  drawRect = QRect(rect().x(), rect().y(), rect().width()-1, rect().height()-1);
  painter.drawRoundedRect(drawRect, 8, 8, Qt::AbsoluteSize);
  
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(QPen(Qt::lightGray, 2));
  painter.setBrush(QBrush(Qt::transparent));
  drawRect = QRect(rect().x() + 3 + GADGET_EDGE, 
    rect().y() + 3 + GADGET_EDGE, 
    rect().width() - (GADGET_EDGE + GADGET_TOOL_EDGE + 6), 
    rect().height() - (2*GADGET_EDGE + 6));
  painter.drawRoundedRect(drawRect, 8, 8, Qt::AbsoluteSize);

  painter.setRenderHint(QPainter::Antialiasing, false);
  painter.setPen(QPen(Qt::black, 1));
  painter.setBrush(QBrush(backColor));
  drawRect = QRect(rect().x() + 3 + GADGET_EDGE, 
    rect().y() + 3 + GADGET_EDGE, 
    rect().width() - (GADGET_EDGE + GADGET_TOOL_EDGE + 7), 
    rect().height() - (2*GADGET_EDGE + 7));
  painter.drawRoundedRect(drawRect, 8, 8, Qt::AbsoluteSize);
}

/*!
This function overrides the normal widget mouse move event in order to perform 
the moving and resizing.
*/
void Gadget::mouseMoveEvent(QMouseEvent *event)
{
  if (dragging)
  {
    QPoint newLocation = pos();
    newLocation += event->pos();
    newLocation -= dragStartPoint;

    //handle the margins
    QPoint endPoint;
    endPoint.setX(QApplication::desktop()->availableGeometry().width() -
      rect().width());
    endPoint.setY(QApplication::desktop()->availableGeometry().height() -
      rect().height());

    if (newLocation.x() < GADGET_CLIP_EDGE && newLocation.x() > - GADGET_CLIP_EDGE)
      newLocation.setX(0);
    if (newLocation.y() < GADGET_CLIP_EDGE && newLocation.y() > - GADGET_CLIP_EDGE)
      newLocation.setY(0);

    if (newLocation.x() > endPoint.x() - GADGET_CLIP_EDGE && newLocation.x() < endPoint.x() + GADGET_CLIP_EDGE)
      newLocation.setX(endPoint.x());
    if (newLocation.y() > endPoint.y() - GADGET_CLIP_EDGE && newLocation.y() < endPoint.y() + GADGET_CLIP_EDGE)
      newLocation.setY(endPoint.y());

    move(newLocation);
  }
  else if (resizing)
  {
    QPoint newLocation = pos();
    newLocation += event->pos();
    newLocation -= dragStartPoint;

    //handle the margins
    QPoint endPoint;
    endPoint.setX(QApplication::desktop()->availableGeometry().width() -
      rect().width());
    endPoint.setY(QApplication::desktop()->availableGeometry().height() -
      rect().height());

    if (newLocation.x() < GADGET_CLIP_EDGE && newLocation.x() > - GADGET_CLIP_EDGE)
      newLocation.setX(0);
    if (newLocation.y() < GADGET_CLIP_EDGE && newLocation.y() > - GADGET_CLIP_EDGE)
      newLocation.setY(0);

    if (newLocation.x() > endPoint.x() - GADGET_CLIP_EDGE && newLocation.x() < endPoint.x() + GADGET_CLIP_EDGE)
      newLocation.setX(endPoint.x());
    if (newLocation.y() > endPoint.y() - GADGET_CLIP_EDGE && newLocation.y() < endPoint.y() + GADGET_CLIP_EDGE)
      newLocation.setY(endPoint.y());

    resize(dragStartSize.width() + event->pos().x() - dragStartPoint.x(), dragStartSize.height() + event->pos().y() - dragStartPoint.y());
  }

  QWidget::mouseMoveEvent(event);
}

/*!
This function overrides the normal widget mouse press event in order to perform 
the moving and resizing.
*/
void Gadget::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    if ((event->x() > size().width() - GADGET_TOOL_EDGE) &&
      ((event->y() > size().height() - GADGET_TOOL_EDGE)))
    {
      dragStartSize = size();
      dragStartPoint = event->pos();
      resizing = true;
    }
    else
    {
      dragStartPoint = event->pos();
      dragging = true;
    }
  }

  QWidget::mousePressEvent(event);
}

/*!
This function overrides the normal widget mouse release event in order to perform 
the moving and resizing.
*/
void Gadget::mouseReleaseEvent(QMouseEvent *event)
{
  dragging = false;
  resizing = false;

  QWidget::mouseReleaseEvent(event);
}

/*!
This function overrides the normal widget resize event in order to perform 
the resizing and positioning of the toolbar.
*/
void Gadget::resizeEvent(QResizeEvent *event)
{
  int xLocation, yLocation;
  xLocation = rect().width() - 25;
  yLocation = 8;
  for (int i = 0; i < toolbox.count(); i++)
  {
    toolbox[i]->move(xLocation, yLocation);
    yLocation += toolbox[i]->height() + 2;
  }

  QWidget::resizeEvent(event);
}

/*!
This function overrides the normal widget event loop in order to perform the 
animation on mouse hover enter and leave.
*/
bool Gadget::event(QEvent *event)
{
  if (event->type() == QEvent::HoverEnter) 
  {
    animate(300, 1.0);
  }
  else if (event->type() == QEvent::HoverLeave)
  {
    animate(300, 0.1);
  }

  return QWidget::event(event);
}
