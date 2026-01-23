#pragma once
#include <Model/Repositories/BaseRepository.h>
#include <Model/Repositories/CategoryRepository.h>

class TransactionRepository : public BaseRepository
{
public:
    TransactionRepository() = default;

    QVector<Transaction> getAll() const;

    bool addTransaction(const Transaction& transaction) const;

    bool removeTransactionById(int id) const;

    QVector<Transaction> getAllProfileTransaction(int profileId) const;

    bool updateTransaction(const Transaction& transaction) const;
    Transaction getTransactionById(int id) const;

    double getMonthlyExpenses(int profileId, int month, int year) const;

    double getSumByTypeAndDate(int profileId, const QString& type, const QDate& start, const QDate& end) const;

    double getAllTimeMonthlyAverageExpense(int profileId) const;

    QMap<int, double> getExpensesByCategory(int userId, const QDate& start, const QDate& end) const;
};