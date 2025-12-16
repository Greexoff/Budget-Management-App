#pragma once
#include <Model/Repositories/BaseRepository.h>
#include <Model/Repositories/CategoryRepository.h>

class TransactionRepository : public BaseRepository
{
public:
    TransactionRepository() = default;

    QVector<Transaction> getAll() const;

    bool addTransaction(const Transaction& transaction);

    bool removeTransactionById(int id);

    QVector<Transaction> getAllProfileTransaction(int profileId) const;

    bool updateTransaction(const Transaction& transaction);
    Transaction getTransactionById(int id) const;

    double getMonthlyExpenses(int profileId, int month, int year) const;
};