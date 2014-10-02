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
#include "Button.h"

/*!
Create a new instance of the Button class.
*/
Button::Button(QWidget *parent) : QLabel(parent)
{
  QStateMachine *machine = new QStateMachine(this);

  QState *s1 = new QState();
  s1->assignProperty(this, "state", Outside);

  QState *s2 = new QState();
  s2->assignProperty(this, "state", Inside);

  QEventTransition *enterTransition = new QEventTransition(this, QEvent::Enter);
  enterTransition->setTargetState(s2);
  s1->addTransition(enterTransition);

  QEventTransition *leaveTransition = new QEventTransition(this, QEvent::Leave);
  leaveTransition->setTargetState(s1);
  s2->addTransition(leaveTransition);

  QState *s3 = new QState();
  s3->assignProperty(this, "state", Pressed);

  QEventTransition *pressTransition = new QEventTransition(this, QEvent::MouseButtonPress);
  pressTransition->setTargetState(s3);
  s2->addTransition(pressTransition);

  QEventTransition *releaseTransition = new QEventTransition(this, QEvent::MouseButtonRelease);
  releaseTransition->setTargetState(s2);
  s3->addTransition(releaseTransition);

  machine->addState(s1);
  machine->addState(s2);
  machine->addState(s3);

  machine->setInitialState(s1);
  machine->start();

  buttonState = Outside;
  color1 = Qt::transparent;
  color2 = Qt::transparent;
  color3 = QColor::fromRgb(255, 255, 180, 255);
  color4 = QColor::fromRgb(255, 128, 0, 255);
  color5 = QColor::fromRgb(255, 255, 180, 255);
  color6 = QColor::fromRgb(255, 128, 0, 255);

  optimumSize = QSize(100, 100);

  setAttribute(Qt::WA_Hover, true);
}

/*!
Clean up.
*/
Button::~Button()
{

}

/*!
Set the current state of the button.
*/
void Button::setState(const Button::ButtonState &newButtonState)
{
  if (buttonState != newButtonState)
  {
    buttonState = newButtonState;
    update();
  }
}

/*!
Get the current state of the button.
*/
Button::ButtonState Button::getState()
{
  return buttonState;
}

/*!
Set the button tag.
\param newTag the new tag.
*/
void Button::setTag(const QString &newTag)
{
  tag = newTag;
}

/*!
Get the button tag.
*/
QString Button::getTag()
{
  return tag;
}

/*!
Set the optimum size of the button using a QSize.
\param size the optimum size.
*/
void Button::setOptimumSize(const QSize& size)
{
  optimumSize = size;
  resize(size);
}

/*!
Set the optimum size of the button using the width and hight.
\param w the optimum width.
\param h the optimum hight.
*/
void Button::setOptimumSize (int w, int h)
{
  setOptimumSize(QSize(w,h));
}

/*!
Get the size hint for the button.
*/
QSize Button::sizeHint() const
{
  return optimumSize;
}

/*!
Set the pixmap to be displayed.
\param pixmap the pixmap.
\note The optimum size will be set to match the size of the pixmap.
*/
void Button::setPixmap(QPixmap pixmap)
{
  buttonPixmap = pixmap;
  setOptimumSize(buttonPixmap.size());
}

/*!
Set the colors used to display the button. The colors are used in pairs to draw
a vertical gradient for background. One pair is used for each state.
\param color1 first color for the Button::Outside state.
\param color2 second color for the Button::Outside state.
\param color3 first color for the Button::Inside state.
\param color4 second color for the Button::Inside state.
\param color5 first color for the Button::Pressed state.
\param color6 second color for the Button::Pressed state.
*/
void Button::setColors(QColor color1, QColor color2, QColor color3,
                       QColor color4, QColor color5, QColor color6)
{
  this->color1 = color1;
  this->color2 = color2;
  this->color3 = color3;
  this->color4 = color4;
  this->color5 = color5;
  this->color6 = color6;
}

/*!
Override the mouse release event. Emits the clicked signal if the release event
happens in the widget's rect.
*/
void Button::mouseReleaseEvent(QMouseEvent * event)
{
  if (event->button() == Qt::LeftButton)
  {
    if (rect().contains(event->pos()))
      emit clicked();
  }
}

/*!
Override the paint event in order to fill the background display the pixmap and 
call the QLabel paint event to draw the text.
*/
void Button::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  QLinearGradient linearGradient;
  QPen pen;
  QRect drawRect;

  linearGradient = QLinearGradient(0, 0, 0, rect().height() + 1);

  switch (buttonState)
  {
  case Outside:
    {
      linearGradient.setColorAt(0.0, color1);
      linearGradient.setColorAt(1.0, color2);
      pen = QPen(Qt::transparent, 1);
      break;
    }
  case Inside:
    {
      linearGradient.setColorAt(0.0, color3);
      linearGradient.setColorAt(1.0, color4);
      pen = QPen(Qt::gray, 1);
      break;
    }
  case Pressed:
    {
      linearGradient.setColorAt(0.0, color5);
      linearGradient.setColorAt(0.8, color6);
      pen = QPen(Qt::gray, 1);
      break;
    }
  }

  painter.setRenderHint(QPainter::Antialiasing, false);
  painter.setPen(pen);
  painter.setBrush(linearGradient);
  drawRect = QRect(rect().x(), rect().y(), rect().width()-1, rect().height()-1);
  painter.drawRoundedRect(drawRect, 2, 2, Qt::AbsoluteSize);

  painter.drawPixmap((width()-buttonPixmap.size().width())/2.0,
    (height()-buttonPixmap.size().height())/2.0, 
    buttonPixmap);

  QLabel::paintEvent(event);
}
