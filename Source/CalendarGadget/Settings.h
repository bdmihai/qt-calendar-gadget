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

#ifndef SETTINGS_H
#define SETTINGS_H

/*!
This class represents the settings class for the application.
*/
class Settings : public QSettings
{
  Q_OBJECT

  public:
    Settings(QString filePath);
    virtual ~Settings();

    void init();

    void setPosition(const QPoint &newPosition);
    QPoint getPosition();
    void setSize(const QSize &newSize);
    QSize getSize();

    QString getRefreshToken();
    void setRefreshToken(const QString &newRefreshToken);

    QString getClientId();
    QString getClientSecret();

  private:
    void setDefault();
    QString encrypt(const QString &text);
    QString decrypt(const QString &text);
};

extern Settings settings;

#endif
