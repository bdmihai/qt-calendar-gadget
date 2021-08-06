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
#include "Defines.h"
#include "Settings.h"
#include "Calendar.h"
#include "ConnectDialog.h"

PowerbroadcastEventFilter::PowerbroadcastEventFilter()
{
    qDebug() << "PowerbroadcastEventFilter::PowerbroadcastEventFilter()";
}

void PowerbroadcastEventFilter::setCalendardReference(Calendar* calendar)
{
    this->calendar = calendar;
}

bool PowerbroadcastEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
// Q_DECL_OVERRIDE 
{
    MSG* msg = static_cast<MSG*>(message);

    // https://docs.microsoft.com/en-us/windows/win32/power/wm-powerbroadcast
    if (msg->message == WM_POWERBROADCAST) {
        switch (msg->wParam) {
        case PBT_APMPOWERSTATUSCHANGE:
            qDebug() << ("PBT_APMPOWERSTATUSCHANGE received- Notifies applications of a change in the power status of the computer, such as a switch from battery power to A/C. The system also broadcasts this event when remaining battery power slips below the threshold specified by the user or if the battery power changes by a specified percentage.\n");
            break;
        case PBT_APMRESUMEAUTOMATIC:
            qDebug() << ("PBT_APMRESUMEAUTOMATIC received - Notifies applications that the system is resuming from sleep or hibernation. This event is delivered every time the system resumes and does not indicate whether a user is present.\n");
            // A PBT_APMRESUMEAUTOMATIC message arrives whenever the system resumes. 
            // We need to re-create the calendar service then.
            calendar->refreshGoogleService();
            break;
        case PBT_APMRESUMESUSPEND:
            qDebug() << ("PBT_APMRESUMESUSPEND received - Notifies applications that the system has resumed operation after being suspended.\n");
            break;
        case PBT_APMSUSPEND:
            qDebug() << ("PBT_APMSUSPEND received - Notifies applications that the computer is about to enter a suspended state. This event is typically broadcast when all applications and installable drivers have returned TRUE to a previous PBT_APMQUERYSUSPEND event.\n");
            break;
        case PBT_POWERSETTINGCHANGE:
            qDebug() << ("PBT_POWERSETTINGCHANGE received - Power setting change event sent with a WM_POWERBROADCAST window message or in a HandlerEx notification callback for services.\n");
            break;
        }
    }
    // An application should return TRUE if it processes this message.
    // If false is returned the message is still being processed by the system.
    return false;
}


/*!
Create a new instance of the Calendar class.
*/
Calendar::Calendar(QWidget *parent) : Gadget(parent)
{
  // create the calendar service
  googleService = new GoogleService(this);

  // create the base layout
  createLayout();

  // update timer
  currentDate = QDate::currentDate();
  displayDate = currentDate;
  currentTime = QTime::currentTime();

  updateTimer.setInterval(10000);
  updateTimer.start();

  connect(&updateTimer, SIGNAL(timeout()), this, SLOT(updateCurrent()));
  calculatedDay = QDate(1473, 2, 19);

    // initial update
  updateCurrent();
  updateDisplay();
}

/*!
Clean up.
*/
Calendar::~Calendar()
{
  delete googleService;
}

boolean Calendar::refreshGoogleService()
{
    // Whenever the system resumes we need to re-create the calendar service
    if (googleService != NULL) delete googleService;
    googleService = NULL;
    googleService = new GoogleService(this);
    if (googleService != NULL) return true;
    else return false;
}

/*!
Create the calendar basic layout.
*/
void Calendar::createLayout()
{
  QVBoxLayout *verticalLayout = new QVBoxLayout();
  QHBoxLayout *horizontalLayout = new QHBoxLayout();

  prevMonth = new Button(this);
  prevMonth->setPixmap(QPixmap (":/PrevMonth.png"));
  prevMonth->setOptimumSize(25, 25);
  prevMonth->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  currentMonth = new DropDown(this);
  currentMonth->setAlignment(Qt::AlignCenter);
  currentMonth->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  currentMonth->setOptimumSize(0, 25);
  connect(currentMonth, SIGNAL(aboutToHide()), this, SLOT(aboutToHideMenu()));
  connect(currentMonth, SIGNAL(aboutToShow()), this, SLOT(updateMonths()));

  currentYear = new DropDown(this);
  currentYear->setAlignment(Qt::AlignCenter);
  currentYear->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  currentYear->setOptimumSize(70, 25);
  connect(currentYear, SIGNAL(aboutToHide()), this, SLOT(aboutToHideMenu()));
  connect(currentYear, SIGNAL(aboutToShow()), this, SLOT(updateYears()));

  nextMonth = new Button(this);
  nextMonth->setPixmap(QPixmap (":/NextMonth.png"));
  nextMonth->setOptimumSize(25, 25);
  nextMonth->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  daysTable = new DaysTable(this);
  daysTable->resize(299, 158);
  daysTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  connect(daysTable, SIGNAL(requestEvents(const QDateTime &, const QDateTime &)),
          googleService, SLOT(getEvents(const QDateTime &, const QDateTime &)));
  connect(googleService, SIGNAL(eventAvailable(EventItem)),
          daysTable, SLOT(displayEvent(EventItem)));

  todayButton = new Button(this);
  todayButton->setAlignment(Qt::AlignCenter);
  todayButton->setOptimumSize(299, 25);
  todayButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  connectButton = new Button(this);
  connectButton->setToolTip(tr("Connect to Google Calendar service"));
  connectButton->setPixmap(QPixmap (":/Google.png"));
  connect(connectButton, SIGNAL(clicked()), this, SLOT(showConnect()));

  aboutButton = new Button(this);
  aboutButton->setToolTip(tr("About"));
  aboutButton->setPixmap(QPixmap (":/About.png"));
  connect(aboutButton, SIGNAL(clicked()), this, SLOT(showAbout()));

  toolbox.append(connectButton);
  toolbox.append(aboutButton);

  horizontalLayout->setContentsMargins(0, 0, 0, 0);
  horizontalLayout->setSpacing(2);
  verticalLayout->setContentsMargins(0, 0, 0, 0);
  verticalLayout->setSpacing(6);
  horizontalLayout->addWidget(prevMonth);
  horizontalLayout->addWidget(currentMonth);
  horizontalLayout->addWidget(currentYear);
  horizontalLayout->addWidget(nextMonth);
  verticalLayout->addLayout(horizontalLayout);
  verticalLayout->addWidget(daysTable);
  verticalLayout->addWidget(todayButton);

  setLayout(verticalLayout);

  connect(prevMonth, SIGNAL(clicked()), this, SLOT(goToPrevMonth()));
  connect(nextMonth, SIGNAL(clicked()), this, SLOT(goToNextMonth()));
  connect(todayButton, SIGNAL(clicked()), this, SLOT(goToToday()));

  setWindowTitle("Calendar");
  setWindowIcon(QIcon(":/AppIcon.png"));
}

/*!
This function overrides the normal widget show event in order to load the
settings and perform event driven tasks.
*/
void Calendar::showEvent(QShowEvent *event)
{
  move(settings.getPosition());
  resize(settings.getSize());

  Gadget::showEvent(event);
}

/*!
This function overrides the normal widget close event in order to save the
settings and close the entire application.
*/
void Calendar::closeEvent(QCloseEvent *event)
{
  if (canClose)
  {
    settings.setPosition(pos());
    settings.setSize(size());
    qApp->quit();
  }
  Gadget::closeEvent(event);
}

void Calendar::setCurrentDate(QDate date)
{
    displayDate = date;
}

void Calendar::setCalculatedDay(QDate date)
{
    calculatedDay = date;
}

QDate Calendar::getCalculatedDay()
{
    return calculatedDay;
}

PowerbroadcastEventFilter* Calendar::getPowerbroadcastEventFilter()
{
    return &powerbroadcastEventFilter;
}


/*!
Updates the months to the month selection drop down.
*/
void Calendar::updateMonths()
{
  currentMonth->clear();

  for (int i = 1; i <= 12; i++)
  {
    Button *button = currentMonth->addButton(QDate::longMonthName(i));
    connect(button, SIGNAL(clicked()), this, SLOT(goToMonth()));
  }
}

/*!
Updates the years to the year selection drop down.
*/
void Calendar::updateYears()
{
  currentYear->clear();

  for (int i = - 20; i <= -10; i+=10)
  {
    QString tempStr = QString("%1").arg(displayDate.year() + i);
    Button *button = currentYear->addButton(tempStr);
    connect(button, SIGNAL(clicked()), this, SLOT(goToYear()));
  }
  for (int i = - 3; i <= 3; i++)
  {
    QString tempStr = QString("%1").arg(displayDate.year() + i);
    Button *button = currentYear->addButton(tempStr);
    connect(button, SIGNAL(clicked()), this, SLOT(goToYear()));
  }
  for (int i = 10; i <= 20; i+=10)
  {
    QString tempStr = QString("%1").arg(displayDate.year() + i);
    Button *button = currentYear->addButton(tempStr);
    connect(button, SIGNAL(clicked()), this, SLOT(goToYear()));
  }
}

/*!
Display the next month.
Day of the month is set to 1 avoiding days that are not available for the selected month.
*/
void Calendar::goToPrevMonth()
{
  daysTable->animateLeft();
  displayDate.setDate(displayDate.year(),
                      displayDate.month(),
                      1);
  displayDate = displayDate.addMonths(-1);
  updateDisplay();
}

/*!
Display the previous month.
Day of the month is set to 1 avoiding days that are not available for the selected month.
*/
void Calendar::goToNextMonth()
{
  daysTable->animateRight();
  displayDate.setDate(displayDate.year(),
                      displayDate.month(),
                      1);
  displayDate = displayDate.addMonths(1);
  updateDisplay();
}

/*!
Display the selected month.
Day of the month is set to 1 avoiding days that are not available for the selected month.
*/
void Calendar::goToMonth()
{
  Button *button = qobject_cast<Button *>(sender());
  if (button)
  {
    daysTable->animateTop();
    displayDate.setDate(displayDate.year(),
                        QDate().fromString(button->getTag(), "MMMM").month(),
                        1);
    updateDisplay();
  }
}

/*!
Display the selected year.
Day of the month is set to 1 avoiding days that are not available for the selected month.
*/
void Calendar::goToYear()
{
  Button *button = qobject_cast<Button *>(sender());
  if (button)
  {
    daysTable->animateTop();
    displayDate.setDate(button->getTag().toInt(),
                        displayDate.month(),
                        1);
    updateDisplay();
  }
}

/*!
Display the calendar for the current date.
*/
void Calendar::goToToday()
{
  daysTable->animateBottom();
  displayDate = currentDate;
  updateDisplay();
}

/*!
Shows the connection dialog.
*/
void Calendar::showConnect()
{
  // check if we have ssl support (mandatory for Google)
  if (QSslSocket::supportsSsl())
  {
    ConnectDialog *connectDialog;
    connectDialog = new ConnectDialog(this);

    connectDialog->setAuthRequestUrl(googleService->getAuthRequestUrl());
    if (connectDialog->exec() == QDialog::Accepted)
    {
      if (!googleService->login(connectDialog->getAuthCode()))
      {
        QMessageBox::information(this,
                                 tr("Calendar Gadget"),
                                 tr("<b>OAuth2 code exchange error: </b>") + googleService->getLoginError());
      }
    }
  }
  else
  {
    QMessageBox::information(this,
                             tr("Calendar Gadget"),
                             tr("<b>Calendar Gadget</b> requires SSL support \r\n") +
                             tr("to connect with Google!"));
  }

  daysTable->displayDate(displayDate);
}

/*!
Shows the about dialog.
*/
void Calendar::showAbout()
{
  QMessageBox::about(this, tr("About Calendar Gadget"),
                     tr("<b>Calendar Gadget</b> version ")  + APP_VERSION +
                     tr("<p>Developed using <a href=\"http://www.qt.io\">Qt ")
                     + qVersion() + tr(" framework</a>.</p>"
                                       "<p>Email : <a href=\"mailto:bdmihai@gmail.com\">bdmihai@gmail.com</a></p>"));
}

/*!
Update the "Today" button text. At midnight update the display to the current
date.
*/
void Calendar::updateCurrent()
{
  QString todayString;
  QDate oldDate;

  oldDate = currentDate;
  currentDate = QDate::currentDate();
  currentTime = QTime::currentTime();

  todayString = QString("<font size=\"4\" color=\"black\" face=\"Verdana\">%1, %2 %3, %4 %5</font></color>").
                arg(QDate::longDayName(currentDate.dayOfWeek())).
                arg(QDate::longMonthName(currentDate.month())).
                arg(currentDate.day()).
                arg(currentDate.year()).
                arg(currentTime.toString("h:mm AP"));

  todayButton->setText(todayString);

  // handle day change at midnight
  if (oldDate.day() != currentDate.day())
    updateDisplay();
}

/*!
Update the display.
*/
void Calendar::updateDisplay()
{
  QString monthText;
  QString yearText;
  QDate   tempDate;
  QDate   firstDate;

  monthText = QString("<font size=\"5\" color=\"black\" face=\"Verdana\">%1</font></color>").
              arg(QDate::longMonthName(displayDate.month()));

  yearText = QString("<font size=\"5\" color=\"black\" face=\"Verdana\">%1</font></color>").
             arg(displayDate.year());

  currentMonth->setText(monthText);
  currentYear->setText(yearText);

  daysTable->displayDate(displayDate);
}
