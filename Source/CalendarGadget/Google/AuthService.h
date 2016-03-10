/****************************************************************************
**
** Copyright (C) 2010-2016 B.D. Mihai.
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
https://www.openssl.org/community/binaries.html
The necessary documentation for using this service can be found at Google:
https://developers.google.com/identity/protocols/OAuth2InstalledApp
*/
class AuthService : public QObject
{
    Q_OBJECT

  public:
    AuthService(QObject *parent);
    virtual ~AuthService();

    QUrl getAuthRequestUrl();
    QString getAccessToken();
    QString getError();
    bool hasRefreshToken();

    bool exchangeCodeForToken(const QString &code);

  public slots:
    bool refreshToken();

  private slots:
    void exchangeCodeForTokenFinished();
    void refreshTokenFinished();

  private:
    QString accessToken;
    QString error;
    QEventLoop loop;
    QTimer accessTokenTimer;
    QNetworkAccessManager netAccMan;
    QNetworkReply *reply;
};

#endif