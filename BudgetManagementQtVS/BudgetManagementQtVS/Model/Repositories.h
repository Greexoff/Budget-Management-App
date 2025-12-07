#pragma once

#include <QVector.h>
#include <QString.h>

#include <qsqldatabase.h>
#include <Model/Transaction.h>
#include <Model/DatabaseManager.h>
#include <Model/User&Profiles.h>
#include <Model/Category.h>

class BaseRepository {
public: 
	BaseRepository();
	virtual ~BaseRepository() = default;
protected:
	QSqlDatabase& database;
};

class UserRepository : public BaseRepository
{
public:
	UserRepository() = default;

	int authenticateUser(QString username, QString password) const;
	bool addUser(QString username, QString password);
	bool removeUserById(int userId);
};

class ProfilesRepository : public BaseRepository
{
public:
	ProfilesRepository() = default;
	QVector<Profile> getProfilesByUserId(int userId) const;
	bool addProfile(int userId, QString profileName);
	bool removeProfileById(int profileId);
};

class TransactionRepository : public BaseRepository
{
public:
	TransactionRepository() = default;
	QVector<Transaction> getAll() const;
	bool add(const Transaction& transaction);
	bool removeById(int id);
	QVector<Transaction> getAllForProfile(int profileId) const;
};

class CategoryRepository : public BaseRepository
{
public:
	CategoryRepository() = default;
	QVector<Category> getAllCategories(int profileId) const;
	bool addCategory(const QString& category, int profileId);
	bool removeCategoryUsingId(int categoryId);
	QString getNameOfCategoryBasedOnId(int categoryId);
};

