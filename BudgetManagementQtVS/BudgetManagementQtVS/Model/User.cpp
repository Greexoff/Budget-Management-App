#include "Model/User.h"
/**
 * @brief Constructs a User with specified properties
 * @param userId Unique identifier
 * @param username Login name
 * @param password Authentication password
 */
User::User(int userId, QString username, QString password)
{
	this->userId = userId;
	this->username = username;
	this->password = password;
}

/**
 * @brief Returns the user's unique identifier
 * @return int User ID
 */
int User::getUserId() const
{
	return userId;
}

/**
 * @brief Returns the user's login name
 * @return QString Username
 */
QString User::getUsername() const
{
	return username;
}

/**
 * @brief Returns the user's password
 * @return QString Password (plaintext)
 */
QString User::getPassword() const
{
	return password;
}
