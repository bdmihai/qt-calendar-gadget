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

#ifndef BUTTON_H
#define BUTTON_H

/*!
This class represents a simple button that can display a text or a image.
*/
class Button : public QLabel
{
  Q_OBJECT

  public:
    Button(QWidget *parent = 0);
    virtual ~Button();

    //! This enumeration describes the state of a button in connection to the user actions
    enum ButtonState
    {
      //! 
      Outside = 0,
      Inside  = 1,
      Pressed = 2
    };

    Q_ENUMS(ButtonState)
    Q_PROPERTY(ButtonState state READ getState WRITE setState)

    void setState(const ButtonState &newButtonState);
    ButtonState getState();
    void setTag(const QString &newTag);
    QString getTag();

    void setPixmap(QPixmap pixmap);
    void setColors(QColor color1, QColor color2, QColor color3,
                   QColor color4, QColor color5, QColor color6);

    virtual QSize sizeHint() const;
    virtual void  setOptimumSize (const QSize&);
    virtual void  setOptimumSize (int w, int h);

  signals:
    void clicked();

  protected:
    ButtonState buttonState;
    QPixmap  buttonPixmap;
    QColor color1;
    QColor color2;
    QColor color3;
    QColor color4;
    QColor color5;
    QColor color6;
    QSize  optimumSize;
    QString tag;

    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
};

#endif

