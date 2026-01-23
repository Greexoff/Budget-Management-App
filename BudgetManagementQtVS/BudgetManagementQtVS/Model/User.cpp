/**
 * @file User.cpp
 * @brief Implementation of the User model.
 */
#include "Model/User.h"

 /** @brief Constructor initializing credentials. */
User::User(int userId, QString username, QString password)
{
	this->userId = userId;
	this->username = username;
	this->password = password;
}

/** @brief Getter for User ID. */
int User::getUserId() const
{
	return userId;
}

/** @brief Getter for Username. */
QString User::getUsername() const
{
	return username;
}

/** @brief Getter for Password Hash. */
QString User::getPassword() const
{
	return password;
}
