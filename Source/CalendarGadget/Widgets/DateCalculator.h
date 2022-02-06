#pragma once
#include <QDate>
#include <QRegularExpression>

class DateCalculator
{

	QDate date;

public:
	DateCalculator();
	DateCalculator(QDate d);

	QDate currentDate();
	QDate evaluate(QString expression);
	void assignDate(QDate& d);
	bool evaluateTimePeriod(QString op, QString factor, QString period);
};

