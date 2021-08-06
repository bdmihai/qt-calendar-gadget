#include "StdAfx.h"
#include "DateCalculator.h"
#include <QDebug>
#include "tinyexpr.h"

DateCalculator::DateCalculator()
{
    date = QDate::currentDate();
}

DateCalculator::DateCalculator(QDate d)
{
    date = d;
}

QDate DateCalculator::currentDate()
{
    return date;
}

/*
* Calculate the desired date from this->date and an expression like +10 + 1w + 2m + 3y (i.e. 10 days + one week + two months + one year).
* We want to allow general expressions for the days, e.g. 100/2 + 25/4 + ...  - but fractions of months or years don't make sense.
* If the expression does not evaluate to a reasonable date calculation the date is returned unchanged.
*/
QDate DateCalculator::evaluate(QString expression)
{
    /*
    * So with a suitable regex we collect all matches for the w, m, y variables, then evaluate each of them like this
    *     Group1 = "+"  Group2 = "123", Group3 = "m"  =>   date = date.addMonths(+123)
    * then remove the corresponding match from the string (i.e. overwrite with spaces).
    * What is left at the end is evaluated using tinyexpr, if it works => date = date.addDays(result).
    */

    qDebug().noquote() << "\tDateCalculator::evaluate(" << expression << ")";
    QString CleanedExpression = expression.normalized(QString::NormalizationForm_D);
    CleanedExpression = CleanedExpression.replace(QRegExp("[^a-zA-Z0-9, \t()^*/%+-]"), "");
    if (CleanedExpression != expression) {
        qDebug().noquote() << "\t\tusing CleanedExpression" << CleanedExpression << "instead";
        expression = CleanedExpression;
    }
    QDate originalDate = date;

    // https://doc.qt.io/qt-5/qregularexpression.html
    QRegularExpression re(" *([+-]?) *([^ +-]*[0-9]*) *([wmy])", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatchIterator i = re.globalMatch(expression);  // +0123w - 23m + 44Y  
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next(); 
        QString op = match.captured(1);     // operator == + or - or nothing
        QString factor = match.captured(2); // factor == "0123"
        QString period = match.captured(3); // period == "w", "m", "y"
        qDebug().noquote() << "\tDateCalculator::date is " << this->date.toString(Qt::SystemLocaleDate);
        if (!evaluateTimePeriod(op, factor, period)) {
            qDebug().noquote() << "\tDateCalculator::evaluate(" << expression << ")" << "fails to recognize a valid expression and returns an unchanged date.";
            date = originalDate;
            return date;
        }
        // remove start...end from expression
        int startOffset = match.capturedStart(0);
        int endOffset = match.capturedEnd(0);
        CleanedExpression.replace(startOffset, endOffset - startOffset + 1, QString(endOffset - startOffset + 1,' '));
    }
    qDebug().noquote() << "\tDateCalculator::evaluate(" << expression << ")" << "concludes date" << this->date.toString(Qt::SystemLocaleDate);
    qDebug().noquote() << " \t... and leaves this rest of the expression:" << CleanedExpression; 

    if (!CleanedExpression.simplified().isEmpty()) {
        int err;
        te_expr* n = te_compile(CleanedExpression.toLocal8Bit().constData(), NULL, 0, &err);
        if (n) {
            const double r = te_eval(n);
            te_free(n);
            qDebug().noquote() << "\ttinyExpression result =" << r << "from expression:" << CleanedExpression;
            int scale = 1;
            qDebug().noquote() << "\tthus adding another" << floor(scale * r) << "days";
            assignDate(date.addDays(scale * r));
        }
        else {
            /* Show the user where the error is at. */
            qDebug().noquote().nospace() << "\ttinyExpression error in expression: '" << CleanedExpression << "'";
            QString pos = QString(err - 1, '-');
            qDebug().noquote().nospace() << "\there --------------------------------" << pos << "^" << "  (error position is " << err << ")";
            // printf("\t%*s^\nError near here", err - 1, "");
        }
    }
    return date;
}

void DateCalculator::assignDate(QDate& d)
{
    if (d.isValid()) {
        date = d;
    }
    else {
        qDebug().noquote() << "\tresulting in an invalid (null) date, so returning an unchanged date.";
    }
}

bool DateCalculator::evaluateTimePeriod(QString op, QString factor, QString period)
{
    double f = 0;
    if (factor.isEmpty()) {
        f = 1;
    }
    else {
        f = factor.toInt();
    }
    
    if (f == 0) {
        qDebug().noquote() << "\t\tDateCalculator::evaluateTimePeriod(" << op << "," << factor << "," << period << ") returns" << this->date.toString(Qt::SystemLocaleDate) << "as it reads" << factor << "as 0";
        return false;
    }
    
    int scale = 1;
    if (op == '-') scale = -1;

    if (period.toLower() == "w") {
        if (f <= 100000) assignDate(date.addDays(scale * 7 * f));
        else {
            qDebug().noquote() << "\t\tDateCalculator::evaluateTimePeriod(" << op << "," << factor << "," << period << ") returns" << this->date.toString(Qt::SystemLocaleDate) << "as the number of weeks" << factor << "exceeds 100000";
            return false;
        }
    }
    else if (period.toLower() == "m") {
        if (f <= 10000) assignDate(date.addMonths(scale * f));
        else {
            qDebug().noquote() << "\t\tDateCalculator::evaluateTimePeriod(" << op << "," << factor << "," << period << ") returns" << this->date.toString(Qt::SystemLocaleDate) << "as the number of months" << factor << "exceeds 10000";
            return false;
        }
    }
    else if (period.toLower() == "y") {
        if (f <= 1000) assignDate(date.addYears(scale * f));
        else {
            qDebug().noquote() << "\t\tDateCalculator::evaluateTimePeriod(" << op << "," << factor << "," << period << ") returns" << this->date.toString(Qt::SystemLocaleDate) << "as the number of years" << factor << "exceeds 1000";
            return false;
        }
    }
    else {
        qDebug().noquote() << "\t\tDateCalculator::evaluateTimePeriod() did not see period = w, m, or y but " << period;
        return false;
    }
    qDebug().noquote() << "\t\tDateCalculator::evaluateTimePeriod(" << op << "," << factor << "," << period << ") returns " << this->date.toString(Qt::SystemLocaleDate);
    return true;
}
