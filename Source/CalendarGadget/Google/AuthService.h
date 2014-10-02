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

#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

/*!
This class represents the authentication service necessary for accessing the
other google services. For this class to work a SSL service is required since
this feature is not compiled by default in Qt. OpenSSL is a good option for 
this and a precompiled version for Windows can be found here: 
http://www.openssl.org/related/binaries.html
The necessary documentation for using this service can be found at Google:
http://code.google.com/intl/en/apis/gdata/docs/auth/overview.html#ClientLogin
*/
class AuthService : public QObject
{
  Q_OBJECT

  public:
    AuthService(QNetworkAccessManager *netAccMan, const QString &service);
    virtual ~AuthService();

    virtual bool login(const QString &mail, const QString &password);

    QString getError();
    QString getAuth();
    QString getLsid();
    QString getSid();

  private slots:
    void loginFinished();

  private:
    QString service;
    QString error;
    QString auth;
    QString lsid;
    QString sid;
    QNetworkAccessManager *netAccMan;
    QNetworkReply *reply;
    QEventLoop *loop;

};

#endif