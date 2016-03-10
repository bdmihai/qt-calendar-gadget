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
#include "ConnectDialog.h"

/*! Create a new instance of the ConnectDialog class. */
ConnectDialog::ConnectDialog(QWidget *parent) : QDialog(parent)
{
  authCode = "";
  createLayout();
}

/*! Clean up. */
ConnectDialog::~ConnectDialog()
{
}

/*! Create the layout and connections for the dialog. */
void ConnectDialog::createLayout()
{
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->setContentsMargins(0, 0, 0, 0);
  
  webView = new QWebView(this);
  webView->setZoomFactor(0.8);
  connect(webView, SIGNAL(titleChanged(const QString &)), this, SLOT(titleChanged(const QString &)));
  mainLayout->addWidget(webView);

  // set the layout
  setLayout(mainLayout);
  setWindowTitle(tr("Connect to Google"));
  setWindowFlags(windowFlags() &~ Qt::WindowContextHelpButtonHint );
  setMinimumSize(360, 500);
  setMaximumSize(360, 500);
}

/*! Set the connection url. */
void ConnectDialog::setAuthRequestUrl(const QUrl &url)
{
  webView->setUrl(url);
}

/*! Get the authentication code returned by Google. */
QString ConnectDialog::getAuthCode()
{
  return authCode;
}

/*! Monitor the page title change and in case the title is formated as a valid
response for authentication retrieve the code and accept the dialog. */
void ConnectDialog::titleChanged(const QString &title)
{
  // split the title on first '=' sign
  QString key = title.left(title.indexOf('=')).trimmed();

  if (key == "Success code")
  {
    QString value = title.right(title.length() - title.indexOf('=') - 1).trimmed();
    authCode = value;
    QDialog::accept();
  }
}
