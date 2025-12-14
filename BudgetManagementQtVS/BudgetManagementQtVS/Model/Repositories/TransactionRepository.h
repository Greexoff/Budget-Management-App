#pragma once
#include <Model/Repositories/BaseRepository.h>
#include <Model/Repositories/CategoryRepository.h>
/**
 * @class TransactionRepository
 * @brief Handles database operations for financial transaction management
 */
class TransactionRepository : public BaseRepository
{
public:
    TransactionRepository() = default;

    /**
     * @brief Retrieves all transactions from the database
     * @return Vector of all Transaction objects
     */
    QVector<Transaction> getAll() const;

    /**
     * @brief Adds a new transaction to the database
     * @param transaction Transaction object to add
     * @return True if transaction added successfully, false otherwise
     */
    bool addTransaction(const Transaction& transaction);

    /**
     * @brief Deletes a transaction by ID
     * @param id ID of transaction to delete
     * @return True if transaction deleted successfully, false otherwise
     */
    bool removeTransactionById(int id);

    /**
     * @brief Retrieves all transactions for a specific profile
     * @param profileId ID of the profile
     * @return Vector of Transaction objects belonging to the profile
     */
    QVector<Transaction> getAllProfileTransaction(int profileId) const;

    //edycja transakcji
    bool updateTransaction(const Transaction& transaction);
    Transaction getTransactionById(int id) const;

    double getMonthlyExpenses(int profileId, int month, int year) const;
};