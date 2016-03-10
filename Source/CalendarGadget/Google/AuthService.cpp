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

#include "StdAfx.h"
#include "AuthService.h"
#include "Settings.h"

/*!
Create a new instance of the AuthService.
*/
AuthService::AuthService(QObject *parent) :
  QObject(parent),
  accessTokenTimer(parent),
  netAccMan(parent),
  loop(parent)
{
  accessToken = "";
  error = "";
  reply = 0;

  // setup a timer to refresh the access token
  accessTokenTimer.setSingleShot(true);
  connect(&accessTokenTimer, SIGNAL(timeout()),
          this, SLOT(refreshToken()));
}

/*!
Clean up.
*/
AuthService::~AuthService()
{
}

/*!
Get the access token.
*/
QString AuthService::getAccessToken()
{
  return accessToken;
}

/*!
Get the error text if it is the case.
*/
QString AuthService::getError()
{
  return error;
}

/*!
Get the status of the refresh token.
*/
bool AuthService::hasRefreshToken()
{
  return (settings.getRefreshToken() != "");
}

/*!
Get the url for authentication to be presented by a browser to the user.
*/
QUrl AuthService::getAuthRequestUrl()
{
  /*! This endpoint is the target of the initial request. It handles active
  session lookup, authenticating the user, and user consent.*/
  const QString endpoint = "https://accounts.google.com/o/oauth2/v2/auth";

  /*! For installed applications, use a value of code, indicating that the Google
  OAuth 2.0 endpoint should return an authorization code. */
  const QString response_type = "code";

  /*! This value signals to the Google Authorization Server that the authorization
  code should be returned in the title bar of the browser, with the page text
  asking the user to close the window. */
  const QString redirect_uri = "urn:ietf:wg:oauth:2.0:oob:auto";

  /*! Read-only access to Calendars. */
  const QString scope = "https://www.googleapis.com/auth/calendar.readonly";

  // build up the query for url
  QUrlQuery urlQuery;
  urlQuery.addQueryItem("scope", scope);
  urlQuery.addQueryItem("redirect_uri", redirect_uri);
  urlQuery.addQueryItem("response_type", response_type);
  urlQuery.addQueryItem("client_id", settings.getClientId());

  // build up the url for the request
  QUrl url(endpoint);
  url.setQuery(urlQuery);

  return url;
}

/*!
The response to the initial authentication request includes an authorization
code, which is exchanged for an access token and a refresh token. To make this
token request, an HTTP POST request is sent.
/param code The authorization code returned from the initial request.
*/
bool AuthService::exchangeCodeForToken(const QString &code)
{
  /*! This endpoint is the target of the excenge request.*/
  const QString endpoint = "https://www.googleapis.com/oauth2/v4/token";

  /*! Data type for the request. */
  const QString content_type = "application/x-www-form-urlencoded";

  /*! As defined in the OAuth 2.0 specification, this field must contain a value
  of authorization_code. */
  const QString grant_type = "authorization_code";

  /*! This value signals to the Google Authorization Server that the authorization
  code should be returned in the title bar of the browser, with the page text
  asking the user to close the window. */
  const QString redirect_uri = "urn:ietf:wg:oauth:2.0:oob:auto";

  // build up the url for the request
  QUrl url(endpoint);

  // build up the data (urlencoded)
  QUrlQuery urlQuery;
  urlQuery.addQueryItem("code", code);
  urlQuery.addQueryItem("client_id", settings.getClientId());
  urlQuery.addQueryItem("client_secret", settings.getClientSecret());
  urlQuery.addQueryItem("redirect_uri", redirect_uri);
  urlQuery.addQueryItem("grant_type", grant_type);

  // set-up the request
  QNetworkRequest request = QNetworkRequest();
  request.setUrl(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, content_type);

  // post request for token
  reply = netAccMan.post(request, urlQuery.toString().toLatin1());
  connect(reply, SIGNAL(finished()), this, SLOT(exchangeCodeForTokenFinished()));

  // wait for the reply
  loop.exec();
  return error.isEmpty();
}

/*!
This method will try to obtain a new access token, by sending an HTTPS POST
request. As long as the user has not revoked the access granted to the
application, the response includes a new access token.
*/
bool AuthService::refreshToken()
{
  /*! This endpoint is the target of the excenge request.*/
  const QString endpoint = "https://www.googleapis.com/oauth2/v4/token";

  /*! Data type for the request. */
  const QString content_type = "application/x-www-form-urlencoded";

  /*! As defined in the OAuth 2.0 specification, this field must contain a value
  of refresh_token. */
  const QString grant_type = "refresh_token";

  // build up the url for the request
  QUrl url(endpoint);

  // build up the data (urlencoded)
  QUrlQuery urlQuery;
  urlQuery.addQueryItem("client_id", settings.getClientId());
  urlQuery.addQueryItem("client_secret", settings.getClientSecret());
  urlQuery.addQueryItem("refresh_token", settings.getRefreshToken());
  urlQuery.addQueryItem("grant_type", grant_type);

  // set-up the request
  QNetworkRequest request = QNetworkRequest();
  request.setUrl(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, content_type);

  // post request for token
  reply = netAccMan.post(request, urlQuery.toString().toLatin1());
  connect(reply, SIGNAL(finished()), this, SLOT(refreshTokenFinished()));

  // wait for the reply
  loop.exec();
  return error.isEmpty();
}

/*!
A successful response is returned as a JSON object, similar to the following:
{
  "access_token"  :"1/fFAGRNJru1FTz70BzhT3Zg",
  "expires_in"    : 3920,
  "token_type"    : "Bearer",
  "refresh_token" : "1/xEoDL4iW3cxlI7yDbSRFYNG01kVKM2C-259HOF2aQbI"
}
*/
void AuthService::exchangeCodeForTokenFinished()
{
  if (!reply->error())
  {
    QJsonParseError parserError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll(), &parserError);

    if (parserError.error == QJsonParseError::NoError)
    {
      QJsonObject jsonObject = jsonResponse.object();

      QJsonValue value = jsonObject["access_token"];
      if (value.isString())
        accessToken = value.toString();

      value = jsonObject["refresh_token"];
      if (value.isString())
        settings.setRefreshToken(value.toString());

      value = jsonObject["expires_in"];
      if (value.isDouble())
      {
        int expTime = value.toInt();
        if (expTime > 5)
          accessTokenTimer.start((expTime - 5) * 1000);
      }

      error = "";
    }
    else
    {
      error = parserError.errorString();
    }
  }
  else
  {
    error = reply->errorString();
  }

  reply->deleteLater();
  reply = 0;
  loop.exit();
}

/*!
A successful response is returned as a JSON object, similar to the following:
{
  "access_token":"1/fFBGRNJru1FQd44AzqT3Zg",
  "expires_in"  :3920,
  "token_type"  :"Bearer"
}
*/
void AuthService::refreshTokenFinished()
{
  if (!reply->error())
  {
    QJsonParseError parserError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll(), &parserError);

    if (parserError.error == QJsonParseError::NoError)
    {
      QJsonObject jsonObject = jsonResponse.object();

      QJsonValue value = jsonObject["access_token"];
      if (value.isString())
        accessToken = value.toString();

      value = jsonObject["expires_in"];
      if (value.isDouble())
      {
        int expTime = value.toInt();
        if (expTime > 5)
          accessTokenTimer.start((expTime - 5) * 1000);
      }

      error = "";
    }
    else
    {
      error = parserError.errorString();
    }
  }
  else
  {
    error = reply->errorString();
  }

  reply->deleteLater();
  reply = 0;
  loop.exit();
}
