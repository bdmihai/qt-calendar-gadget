/****************************************************************************
**
** Copyright (C) 2010-2022 B.D. Mihai.
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
#include "Defines.h"
#include "Settings.h"
#include "Widgets\Calendar.h"

//! The application settings.
Settings settings(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" + APP_NAME + "/" + "Settings.ini");

/*!
This is the main entry point for the application. It shows the main window and
starts the application main message loop.
*/
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  // initialize the settings in the main function
  settings.init();

  // set the application info
  app.setApplicationName(APP_NAME);
  app.setOrganizationName(APP_COMPANY);
  app.setOrganizationDomain(APP_DOMAIN);
  app.setApplicationVersion(APP_VERSION);

  // detect the system proxy settings - might take a bit of time
  QNetworkProxyFactory::setUseSystemConfiguration(true);

  // show the main window
  Calendar calendar;
  calendar.show();
  calendar.getPowerbroadcastEventFilter()->setCalendardReference(&calendar);
  app.installNativeEventFilter(calendar.getPowerbroadcastEventFilter()); //installs event filter inside the calendar widget, on the app application instance

  // run the application
  app.setQuitOnLastWindowClosed(false);  // avoid closing on showing a dialog e.g. QMessageBox
  return app.exec();
}
