#include "Model/User.h"

User::User(int userId, QString username, QString password)
{
	this->userId = userId;
	this->username = username;
	this->password = password;
}

int User::getUserId() const
{
	return userId;
}

QString User::getUsername() const
{
	return username;
}

QString User::getPassword() const
{
	return password;
}
