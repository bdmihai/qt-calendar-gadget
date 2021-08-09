CalendarGadget
==============
![Appearance](https://github.com/ThomasHeinrichSchmidt/qt-calendar-gadget/blob/master/Doc/qt-calendar-gadget.png?raw=true)

*A simple compact calendar that displays discreetly on the desktop, similar to a Vista or Windows 7 gadget, but also works for Windows XP and Windows 10.*

After logging in to your Google account (clicking on G on the right) you will be able to see your calendar entries. You will need to login only once as CalendarGadget remembers your login (safely stored in an encrypted token).

Right-clicking on a date brings up a context menu that allows you to perform some date calculations, such as adding 100 days to the clicked date or a formula for more complicated cases.
![Date calculations](https://github.com/ThomasHeinrichSchmidt/qt-calendar-gadget/blob/master/Doc/qt-calendar-gadget-date-calculations.png?raw=true)

The desired date in each case is clearly displayed (in orange) on the calendar, and a tool tip also displays the performed calculation for verification.

![Target date](https://github.com/ThomasHeinrichSchmidt/qt-calendar-gadget/blob/master/Doc/qt-calendar-gadget-date-calculations-target-date.png?raw=true)

You can use an arithmetic expression to calculate the date you want, for example, if you buy a new 100-pack of your vitamins, you can calculate how long it will last if you take 3 of them a day (`100/3`). If you plan to stop taking them for 3 weeks, you can figure out when you would start again (`100/3 + 3w`). And how long the second 100-pack will last (`100/3 + 3w + 100/3`).
You may only use whole weeks, months, and years in these formulas (fractions of these are not useful, in my opinion).

![Target date](https://github.com/ThomasHeinrichSchmidt/qt-calendar-gadget/blob/master/Doc/qt-calendar-gadget-date-calculations-for-pills.png?raw=true)

## Features
- remains almost hidden until you touch it with the mouse
- easy navigation using the arrows and the month drop down
- shows calendar weeks
- optionally shows Google calendar entries
- able to do date calculations based on formulas with days and full weeks, months, and years

## Acknowledgments
* used tinyexpr from [Lewis Van Winkle](https://github.com/codeplea/tinyexpr) to evaluate date expressions like 100/2 +30
