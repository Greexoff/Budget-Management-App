#pragma once
#include <QString>
#include <QDate>

/**
 * @enum TransactionType
 * @brief Defines the type of financial transaction
 */
enum TransactionType
{
	INCOME,		///< Money received 
	EXPENSE		///< Money spent
};

/**
 * @class Transaction
 * @brief Represents a financial transaction in the budgeting system
 *
 * This class encapsulates all data related to a financial transaction,
 * including amount, date, description, category, and associated profile.
 * Transactions are classified as either INCOME or EXPENSE based on
 * the sign of the amount.
 */
class Transaction
{
private:
	int transactionId;						///< Unique transaction identifier
	QString transactionName;				///< Name for the transaction
	QDate transactionDate;					///< Date when transaction occurred
	QString transactionDescription;			///< Additional details about the transaction
	double transactionAmount;				///< Transaction amount
	TransactionType transactionType;		///< Classification as INCOME or EXPENSE
	int categoryId;							///< ID of associated category for classification
	int associatedProfileId;				///< ID of budget profile this transaction belongs to
public:

	/**
	 * @brief Constructs a Transaction object with all properties
	 * @param TransactionId Unique identifier
	 * @param transactionName name for transaction
	 * @param transactionDate Date of transaction
	 * @param transactionDescription Additional details
	 * @param transactionAmount Financial amount
	 * @param transactionType Classification type
	 * @param categoryId ID of associated category
	 * @param associatedProfileId ID of owning profile
	 */
	Transaction(int TransactionId, QString transactionName, QDate transactionDate, QString transactionDescription, double transactionAmount, TransactionType transactionType, int categoryId, int associadedProfileId);

	/**
	 * @brief Destructor
	 */
	~Transaction();


	// Getters	
	int getTransactionId() const;
	QString getTransactionName() const;
	QDate getTransactionDate() const;
	QString getTransactionDescription() const;
	double getTransactionAmount() const;
	int getCategoryId() const;
	int getAssociatedProfileId() const;

	// Setters	
	void setTransactionId(int id);
	void setTransactionName(const QString& name);
	void setTransactionDate(const QDate& date);
	void setTransactionDescription(const QString& description);
	void setTransactionAmount(double amount);
	void setTransactionType(TransactionType type);
	void setCategoryId(int id);
	void setAssociatedProfileId(int associadedProfileId);

	/**
	 * @brief Updates all transaction properties at once
	 * @param name Updated transaction name
	 * @param date Updated transaction date
	 * @param description Updated transaction description
	 * @param amount Updated transaction amount
	 * @param type Updated transaction type
	 * @param categoryId Updated category ID
	 * @param associatedProfileId Updated profile ID
	 */
	void editTransaction(const QString& name, const QDate& date, const QString& description, double amount, TransactionType type, int categoryId, int associatedProfileId);

};