#include "Model/Profile.h"
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