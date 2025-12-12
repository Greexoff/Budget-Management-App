#pragma once
#include <QString>
#include <QDate>
#include "Model/Transaction.h"

/**
 * @class TransactionBuilder
 * @brief Builder class for creating Transaction objects with fluent interface
 *
 * This class implements the Builder pattern to provide a clean, readable
 * way to construct Transaction objects. 
 */
class TransactionBuilder
{
public:
	/**
	 * @brief Default constructor
	 */
	TransactionBuilder() = default;

	/**
	 * @brief Sets the transaction identifier
	 * @param newId New transaction ID
	 * @return Reference to this builder for method chaining
	 */
	TransactionBuilder& withId(int newId);

	/**
	 * @brief Sets the transaction name
	 * @param newName New transaction name
	 * @return Reference to this builder for method chaining
	 */
	TransactionBuilder& withName(const QString& newName);

	/**
	 * @brief Sets the transaction date
	 * @param newDate New transaction date
	 * @return Reference to this builder for method chaining
	 */
	TransactionBuilder& withDate(const QDate& newDate);

	/**
	 * @brief Sets the transaction description
	 * @param newDescription New transaction description
	 * @return Reference to this builder for method chaining
	 */
	TransactionBuilder& withDescription(const QString& newDescription);

	/**
	 * @brief Sets the transaction amount
	 * @param newAmount New transaction amount
	 * @return Reference to this builder for method chaining
	 */
	TransactionBuilder& withAmount(double newAmount);

	/**
	 * @brief Sets the category identifier
	 * @param newCategoryId New category ID
	 * @return Reference to this builder for method chaining
	 */
	TransactionBuilder& withCategoryId(int newCategoryId);

	TransactionBuilder& withFinancialAccountId(int newFinancialAccountId);

	/**
	 * @brief Sets the profile identifier
	 * @param newProfileId New profile ID
	 * @return Reference to this builder for method chaining
	 */
	TransactionBuilder& withProfileId(int newProfileId);

	/**
	 * @brief Constructs the final Transaction object
	 * @return Completed Transaction object with all configured properties
	 */
	Transaction build() const;
private:
	int id = 0;                         ///< Transaction identifier
	QString name;                       ///< Transaction display name
	QDate date = QDate::currentDate();  ///< Transaction date 
	QString description;                ///< Transaction description
	double amount = 0.0;                ///< Transaction amount
	TransactionType type = EXPENSE;     ///< Transaction type 
	int categoryId = 1;                 ///< Associated category ID 
	int financialAccountId = 1;
	int profileId = -1;                 ///< Associated profile ID
};