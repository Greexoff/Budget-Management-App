#pragma once
#include <QString>
#include <QDate>
class Transaction
{
private:
	//id,nazwe,date,kat,opis,kwota,(typ-klasa)
	int transactionId;
	QString transactionName;
	QDate transactionDate;
	QString transactionDescription;
	double transactionAmount;
public:
	//add,rem,edit
	//Transaction();
	//~Transaction();


};