#include <Model/Repositories/TransactionRepository.h>


/**
 * @brief Retrieves all transactions for a specific profile
 * @param profileId ID of the profile
 * @return Vector of Transaction objects belonging to the profile
 */
QVector<Transaction> TransactionRepository::getAllProfileTransaction(int profileId) const
{
    QVector<Transaction> result;
    QSqlQuery query(database);

    query.prepare(
        "SELECT id, name, date, description, amount, type, category_id, financialAccount_id, profile_id "
        "FROM transactions WHERE profile_id = :profileId"
    );
    query.bindValue(":profileId", profileId);

    if (!query.exec())
    {
        qDebug() << "Transaction retrieval for profile failed:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString dateStr = query.value(2).toString();
        QString description = query.value(3).toString();
        double amount = query.value(4).toDouble();
        QString typeStr = query.value(5).toString();
        int categoryId = query.value(6).toInt();
        int financialAccountId = query.value(7).toInt();
        int profileId = query.value(8).toInt();

        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
        TransactionType type = (typeStr == "INCOME") ? INCOME : EXPENSE;

        Transaction transaction(id, name, date, description, amount, type, categoryId, financialAccountId, profileId);
        result.append(transaction);
    }

    return result;
}

/**
 * @brief Retrieves all transactions from the database
 * @return Vector of all Transaction objects
 */
QVector<Transaction> TransactionRepository::getAll() const
{
    QVector<Transaction> result;

    QSqlQuery query(database);

    if (!query.exec("SELECT id, name, date, description, amount, type, profile_id, category_id, financialAccount_id FROM transactions"))
    {
        qDebug() << "TransactionRepository::getAll error:" << query.lastError().text();
        return result;
    }

    while (query.next())
    {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString dateStr = query.value(2).toString();
        QString description = query.value(3).toString();
        double amount = query.value(4).toDouble();
        QString typeStr = query.value(5).toString();
        int associatedProfileId = query.value(6).toInt();
        int categoryId = query.value(7).toInt();
        int financialAccountId = query.value(8).toInt();
        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
        
             
        TransactionType type = (typeStr == "INCOME") ? INCOME : EXPENSE;

        Transaction transaction(id, name, date, description, amount, type, categoryId,financialAccountId, associatedProfileId);
        result.append(transaction);
    }

    return result;
}

/**
 * @brief Adds a new transaction to the database
 *
 * Determines transaction type based on amount sign:
 * Positive amount: EXPENSE
 * Negative amount: INCOME
 *
 * @param transaction Transaction object to add
 * @return True if transaction added successfully, false otherwise
 */
bool TransactionRepository::addTransaction(const Transaction& transaction)
{
    QSqlQuery query(database);
    query.prepare(
        "INSERT INTO transactions (name, type, date, description, amount, category_id ,financialAccount_id, profile_id) "
        "VALUES (:name, :type, :date, :description, :amount, :category_id, :financialAccount_id, :profile_id)"
    );

    query.bindValue(":name", transaction.getTransactionName());

    QString typeStr = (transaction.getTransactionType() == INCOME) ? "INCOME" : "EXPENSE";
    query.bindValue(":type", typeStr);

    query.bindValue(":date", transaction.getTransactionDate().toString("yyyy-MM-dd"));
    query.bindValue(":description", transaction.getTransactionDescription());
    query.bindValue(":amount", transaction.getTransactionAmount());
    query.bindValue(":profile_id", transaction.getAssociatedProfileId());
    query.bindValue(":category_id", transaction.getCategoryId());
    query.bindValue(":financialAccount_id", transaction.getFinancialAccountId());

    if (!query.exec()) {
        qDebug() << "TransactionRepository::add error:" << query.lastError().text();
        return false;
    }
    return true;
}

/**
 * @brief Deletes a transaction by ID
 * @param id ID of transaction to delete
 * @return True if transaction deleted successfully, false otherwise
 */
bool TransactionRepository::removeTransactionById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM transactions WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "TransactionRepository::removeById error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool TransactionRepository::updateTransaction(const Transaction& transaction)
{
    QSqlQuery query(database);

    query.prepare(
        "UPDATE transactions SET "
        "name=:name, "
        "description=:desc, "
        "date=:date, "          
        "amount=:amount, "
        "category_id=:catId, "
        "type=:type, "
        "financialAccount_id=:financialAccountId "
        "WHERE id=:id"
    );

    query.bindValue(":name", transaction.getTransactionName());
    query.bindValue(":desc", transaction.getTransactionDescription());

    query.bindValue(":date", transaction.getTransactionDate().toString("yyyy-MM-dd")); 

    query.bindValue(":amount", transaction.getTransactionAmount());
    query.bindValue(":catId", transaction.getCategoryId());

    QString typeStr = (transaction.getTransactionType() == INCOME) ? "INCOME" : "EXPENSE";
    query.bindValue(":type", typeStr);

    query.bindValue(":id", transaction.getTransactionId());
    query.bindValue(":financialAccountId", transaction.getFinancialAccountId());

    if (!query.exec()) {
        qDebug() << "TransactionRepo::update error:" << query.lastError().text();
        return false;
    }
    return true;
}

Transaction TransactionRepository::getTransactionById(int id) const
{
    QSqlQuery query(database);
    query.prepare("SELECT id, name, date, description, amount, type, category_id, profile_id, financialAccount_id FROM transactions WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        int tId = query.value(0).toInt();
        QString name = query.value(1).toString();
        QDate date = QDate::fromString(query.value(2).toString(), "yyyy-MM-dd");
        QString desc = query.value(3).toString();
        double amount = query.value(4).toDouble();
        QString typeStr = query.value(5).toString();
        int catId = query.value(6).toInt();
        int profId = query.value(7).toInt();
        int financialAccountId = query.value(8).toInt();
        TransactionType type = (typeStr == "INCOME") ? INCOME : EXPENSE;

        return Transaction(tId, name, date, desc, amount, type, catId, financialAccountId, profId);
    }
    return Transaction(-1, "", QDate(), "", 0, EXPENSE, 1, -1, 1);
}

double TransactionRepository::getMonthlyExpenses(int profileId, int month, int year) const
{
    QSqlQuery query(database);
    query.prepare(
        "SELECT SUM(amount) "
        "FROM transactions "
        "WHERE profile_id = :id "
        "AND strftime('%m', date) = :month AND strftime('%Y', date) = :year"
    );

    query.bindValue(":id", profileId);
    query.bindValue(":month", QString("%1").arg(month, 2, 10, QChar('0')));
    query.bindValue(":year", QString::number(year));

    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}