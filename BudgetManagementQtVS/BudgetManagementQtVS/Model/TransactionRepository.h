#pragma once

#include <qvector.h>
#include <qsqldatabase.h>
#include <Model/Transaction.h>
#include <Model/DatabaseManager.h>

class TransactionRepository
{
public:
	TransactionRepository();

	QVector<Transaction> getAll() const;

	bool add(const Transaction& transaction);

	bool removeById(int id);

private:
	QSqlDatabase database;
};
