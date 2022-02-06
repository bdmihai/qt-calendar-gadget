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

#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

class ConnectDialog : public QDialog
{
    Q_OBJECT

  public:
    ConnectDialog(QWidget *parent);
    virtual ~ConnectDialog();

    void setAuthRequestUrl(const QUrl &url);
    QString getAuthCode();

  protected:
    void createLayout();

  private slots:
    void titleChanged(const QString &title);

  protected:
    QWebEngineView *webView;
    QString authCode;
};

#endif
