#pragma once
#include<QString>

/**
 * @class User
 * @brief Represents a user account in the budgeting application
 *
 * This class encapsulates user authentication data including
 * username, password, and unique identifier.
 */
class User
{
public:
	/**
	 * @brief Constructs a User object
	 * @param userId Unique user identifier
	 * @param username Login name
	 * @param password Authentication password
	 */
	User(int userId, QString username, QString password);

	/**
	 * @brief Returns the user's unique identifier
	 * @return int User ID
	 */
	int getUserId() const;

	/**
	 * @brief Returns the user's password
	 * @return QString Password
	 */
	QString getPassword() const;

	/**
	 * @brief Returns the user's login name
	 * @return QString Username
	 */
	QString getUsername() const;

private:
	int userId;			///< Unique identifier
	QString username;	///< Login name
	QString password;	///< Authentication password
};

/**
 * @class Profile
 * @brief Represents a budget profile belonging to a user
 *
 * Users can create multiple profiles to separate transactions
 */