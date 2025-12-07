#include "Model/User&Profiles.h"

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

/**
 * @brief Constructs a Profile with specified properties
 * @param profileId Unique identifier
 * @param userId ID of owning user
 * @param profileName Display name
 */
Profile::Profile(int profileId, int userId, QString profileName)
{
	this->profileId = profileId;
	this->userId = userId;
	this->profileName = profileName;
}

/**
 * @brief Returns the profile's unique identifier
 * @return int Profile ID
 */
int Profile::getProfileId() const
{
	return profileId;
}

/**
 * @brief Returns the ID of the owning user
 * @return int User ID
 */
int Profile::getUserId() const
{
	return userId;
}

/**
 * @brief Returns the profile's display name
 * @return QString Profile name
 */
QString Profile::getProfileName() const
{
	return profileName;
}