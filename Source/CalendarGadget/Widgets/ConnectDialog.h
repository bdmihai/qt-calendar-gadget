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

#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

class ConnectDialog : public QDialog
{
  Q_OBJECT

  public:
    ConnectDialog(QWidget *parent = 0);
    virtual ~ConnectDialog();

    void setUsername(const QString &userName);
    QString getUsername();
    void setPassword(const QString &password);
    QString getPassword();
    bool isRemembered();
    void setRemembered(bool remembered);
    void setProxyType(QNetworkProxy::ProxyType type);
    QNetworkProxy::ProxyType getProxyType();
    void setProxyAddress(const QString &address);
    QString getProxyAddress();
    void setProxyPort(quint16 port);
    quint16 getProxyPort();


  protected:
    void createLayout();

  protected:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *proxyAddressEdit;
    QLineEdit *proxyPortEdit;
    QCheckBox *rememberCheck;
    QButtonGroup *proxyTypeGroup;
    QDialogButtonBox *dialogButtonBox;
};

#endif

