#include <Model/Repositories/FinancialAccountRepository.h>

QVector<FinancialAccount> FinancialAccountRepository::getAllProfileFinancialAccounts(int profileId) const {
	
	QVector<FinancialAccount> financialAccountsForProfile;

	QSqlQuery query(database);

	query.prepare("SELECT id, financialAccount_name, financialAccount_type, financialAccount_balance, profile_id FROM financialAccount WHERE profile_id = :profile_id OR id = 1");
	query.bindValue(":profile_id", profileId);

	if (!query.exec())
	{
		qDebug() << "FinancialAccountRepository::error: Couldn't get financialAccounts" << query.lastError().text();
		return financialAccountsForProfile;
	}

	while (query.next()) {
		int id = query.value(0).toInt();
		QString financialAccountName = query.value(1).toString();
		QString financialAccountType = query.value(2).toString();
		double financialAccountBalance = query.value(3).toDouble();
		int financialAccountProfileId = query.value(4).toInt();

		FinancialAccount financialAccount(id, financialAccountName, financialAccountType, financialAccountBalance, financialAccountProfileId);
		financialAccountsForProfile.append(financialAccount);

	}

	return financialAccountsForProfile;
}

bool FinancialAccountRepository::addFinancialAccount(const QString& financialAccountName, const QString& financialAccountType, double financialAccountBalance, int profileId) {

	QSqlQuery query(database);

	query.prepare("INSERT INTO financialAccount (financialAccount_name, financialAccount_type, financialAccount_balance, profile_id) VALUES (:name, :type, :balance, :profile_id)");
	query.bindValue(":name", financialAccountName);
	query.bindValue(":type", financialAccountType);
	query.bindValue(":balance", financialAccountBalance);
	query.bindValue(":profile_id", profileId);

	if (!query.exec())
	{
		qDebug() << "FinancialAccountRepository:: error: Couldn't add financialAccount to database" << query.lastError().text();
		return false;
	}

	return true;
}

bool FinancialAccountRepository::removeFinancialAccount(int financialAccountId) {
	
	if (financialAccountId == 1) {
		return false;
	}

	QSqlQuery query(database);

	database.transaction();

	query.prepare("UPDATE transactions SET financialAccount_id = :defaultId WHERE financialAccount_id = :finAccount_id");
	query.bindValue(":defaultId", 1);
	query.bindValue(":finAccount_id", financialAccountId);

	if (!query.exec()) {
		qDebug() << "CategoryRepo::removefinancialAccount update transactions error:" << query.lastError().text();
		database.rollback();
		return false;
	}

	query.prepare("DELETE FROM financialAccount WHERE id = :id");
	query.bindValue(":id", financialAccountId);

	if (!query.exec())
	{
		qDebug() << "CategoryRepository:: error: Couldn't remove financialAccount from database" << query.lastError().text();
		database.rollback();
		return false;
	}

	if (!database.commit()) {
		qDebug() << "FinancialAccountyRepository::removeFinancialAccount commit failed:" << database.lastError().text();
		database.rollback();
		return false;
	}

	return true;
}

bool FinancialAccountRepository::updateFinancialAccount(int financialAccountId, const QString& newName, const QString& newType, double newBalance)
{
	if (financialAccountId == 1) return false;

	QSqlQuery query(database);
	query.prepare("UPDATE financialAccount SET financialAccount_name = :name, financialAccount_type = :type, financialAccount_balance = :balance WHERE id = :id");
	query.bindValue(":name", newName);
	query.bindValue(":type", newType);
	query.bindValue(":balance", newBalance);
	query.bindValue(":id", financialAccountId);

	if (!query.exec()) {
		qDebug() << "FinancialAccountRepo::update error:" << query.lastError().text();
		return false;
	}
	return true;
}

QString FinancialAccountRepository::getFinancialAccountNameById(int financialAccountId) const {

	QSqlQuery query(database);
	QString financialAccountName = "";

	query.prepare("SELECT financialAccount_name FROM financialAccount WHERE id = :id");
	query.bindValue(":id", financialAccountId);

	if (!query.exec())
	{
		qDebug() << "FinancialAccountRepository:: error: Couldn't find valid financial account in database" << query.lastError().text();
		return financialAccountName;
	}

	if (query.next())
	{
		financialAccountName = query.value(0).toString();
	}

	return financialAccountName;
}
