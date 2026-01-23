/**
 * @file TransactionRepository.h
 * @brief Header file for the Transaction Repository.
 */
#pragma once
#include <Model/Repositories/BaseRepository.h>
#include <Model/Repositories/CategoryRepository.h>

 /**
  * @class TransactionRepository
  * @brief Manages database operations for financial transactions.
  */
class TransactionRepository : public BaseRepository
{
public:
    TransactionRepository() = default;
    /**
     * @brief Retrieves all transactions in the database.
     * @return A list of all transactions.
     */
    QVector<Transaction> getAll() const;

    /**
     * @brief Adds a new transaction to the database.
     * @param transaction The Transaction object to add.
     * @return True if successful, false otherwise.
     */
    bool addTransaction(const Transaction& transaction) const;

    /**
     * @brief Removes a transaction by its ID.
     * @param id The ID of the transaction.
     * @return True if successful, false otherwise.
     */
    bool removeTransactionById(int id) const;

    /**
     * @brief Retrieves all transactions for a specific profile.
     * @param profileId The ID of the profile.
     * @return A list of transactions.
     */
    QVector<Transaction> getAllProfileTransaction(int profileId) const;

    /**
     * @brief Updates an existing transaction.
     * @param transaction The Transaction object containing updated data.
     * @return True if successful, false otherwise.
     */
    bool updateTransaction(const Transaction& transaction) const;

    /**
     * @brief Retrieves a single transaction by its ID.
     * @param id The ID of the transaction.
     * @return The Transaction object.
     */
    Transaction getTransactionById(int id) const;

    /**
     * @brief Calculates total expenses for a specific month and year.
     * @param profileId The ID of the profile.
     * @param month The month (1-12).
     * @param year The year.
     * @return The total expense amount.
     */
    double getMonthlyExpenses(int profileId, int month, int year) const;

    /**
     * @brief Calculates the sum of amounts for a specific type and date range.
     * @param profileId The profile ID.
     * @param type The transaction type ("Income" or "Expense").
     * @param start The start date.
     * @param end The end date.
     * @return The calculated sum.
     */
    double getSumByTypeAndDate(int profileId, const QString& type, const QDate& start, const QDate& end) const;

    /**
     * @brief Calculates the average monthly expense across all time.
     * @param profileId The profile ID.
     * @return The average expense amount per month.
     */
    double getAllTimeMonthlyAverageExpense(int profileId) const;

    /**
     * @brief Groups expenses by category within a date range.
     * @param userId The profile ID.
     * @param start The start date.
     * @param end The end date.
     * @return A map where Key is Category ID and Value is total amount.
     */
    QMap<int, double> getExpensesByCategory(int userId, const QDate& start, const QDate& end) const;
};