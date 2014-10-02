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
    ~Settings();

    void setPosition(const QPoint &newPosition);
    QPoint getPosition();
    void setSize(const QSize &newSize);
    QSize getSize();
    void setPassword(const QString &newPassword);
    QString getPassword();
    void setUsername(const QString &newUsername);
    QString getUsername();
    bool isRemembered();
    void setRemembered(bool newRemembered);
    void setProxyType(QNetworkProxy::ProxyType type);
    QNetworkProxy::ProxyType getProxyType();
    void setProxyAddress(const QString &address);
    QString getProxyAddress();
    void setProxyPort(quint16 port);
    quint16 getProxyPort();

  private:
    void setDefault();
    QString encript(const QString &text);
    QString decrypt(const QString &text);
};

#endif
