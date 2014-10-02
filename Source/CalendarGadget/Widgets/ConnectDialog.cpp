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
#include "ConnectDialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) : QDialog(parent)
{
  createLayout();
}

ConnectDialog::~ConnectDialog()
{

}

void ConnectDialog::createLayout()
{
  QVBoxLayout *mainLayout = new QVBoxLayout();
  QVBoxLayout *vLayout;
  QGridLayout *gridLayout;
  QRadioButton *radioButton;
  QGroupBox *groupBox;
  QValidator *validator;

  //create the input fields
  gridLayout = new QGridLayout();
  usernameEdit = new QLineEdit();
  passwordEdit = new QLineEdit();
  passwordEdit->setEchoMode(QLineEdit::Password);

  gridLayout->addWidget(new QLabel(tr("Username:")), 0, 0);
  gridLayout->addWidget(new QLabel(tr("Password:")), 1, 0);
  gridLayout->addWidget(usernameEdit, 0, 1);
  gridLayout->addWidget(passwordEdit, 1, 1);
  mainLayout->addLayout(gridLayout);

  // remember the login data
  rememberCheck = new QCheckBox(tr("Remember me"));
  mainLayout->addWidget(rememberCheck);

  // proxy type settings
  groupBox = new QGroupBox(tr("Proxy settings"));
  vLayout = new QVBoxLayout();
  proxyTypeGroup = new QButtonGroup();
  proxyTypeGroup->addButton(radioButton = new QRadioButton("No Proxy"), QNetworkProxy::NoProxy);
  radioButton->setChecked(true);
  vLayout->addWidget(radioButton);
  proxyTypeGroup->addButton(radioButton = new QRadioButton("Socks5 Proxy"), QNetworkProxy::Socks5Proxy);
  vLayout->addWidget(radioButton);
  proxyTypeGroup->addButton(radioButton = new QRadioButton("Http Proxy"), QNetworkProxy::HttpProxy);
  vLayout->addWidget(radioButton);

  // proxy address and port
  gridLayout = new QGridLayout();
  proxyAddressEdit = new QLineEdit();
  proxyPortEdit = new QLineEdit();
  validator = new QIntValidator(0, 65535, this);
  proxyPortEdit->setValidator(validator);

  gridLayout->addWidget(new QLabel(tr("Address:")), 0, 0);
  gridLayout->addWidget(new QLabel(tr("Port:")), 1, 0);
  gridLayout->addWidget(proxyAddressEdit, 0, 1);
  gridLayout->addWidget(proxyPortEdit, 1, 1);
  vLayout->addLayout(gridLayout);

  groupBox->setLayout(vLayout);
  mainLayout->addWidget(groupBox);

  // create the dialog buttons
  dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
    Qt::Horizontal);
  connect(dialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(dialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
  mainLayout->addStretch();
  mainLayout->addWidget(dialogButtonBox);

  // set the layout
  setLayout(mainLayout);
  setWindowTitle(tr("Connect to Google"));
  setWindowFlags(windowFlags() &~ Qt::WindowContextHelpButtonHint );
  setMinimumWidth (250);
}

void ConnectDialog::setUsername(const QString &userName)
{
  usernameEdit->setText(userName);
}

QString ConnectDialog::getUsername()
{
  return usernameEdit->text();
}

void ConnectDialog::setPassword(const QString &password)
{
  passwordEdit->setText(password);
}

QString ConnectDialog::getPassword()
{
  return passwordEdit->text();
}

bool ConnectDialog::isRemembered()
{
  return rememberCheck->isChecked();
}

void ConnectDialog::setRemembered(bool remembered)
{
  rememberCheck->setChecked(remembered);
}

void ConnectDialog::setProxyType(QNetworkProxy::ProxyType type)
{
  QAbstractButton *button = proxyTypeGroup->button(type);
  if (button != 0)
    button->setChecked(true);
}

QNetworkProxy::ProxyType ConnectDialog::getProxyType()
{
  return (QNetworkProxy::ProxyType)proxyTypeGroup->checkedId();
}

void ConnectDialog::setProxyAddress(const QString &address)
{
  proxyAddressEdit->setText(address);
}

QString ConnectDialog::getProxyAddress()
{
  return proxyAddressEdit->text();
}

void ConnectDialog::setProxyPort(quint16 port)
{
  proxyPortEdit->setText(QString::number(port));
}

quint16 ConnectDialog::getProxyPort()
{
  return proxyPortEdit->text().toUInt();
}
