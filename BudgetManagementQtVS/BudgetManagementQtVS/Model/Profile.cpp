/**
 * @file Profile.cpp
 * @brief Implementation of the Profile model.
 */
#include "Model/Profile.h"

 /** @brief Constructor. */
Profile::Profile(int profileId, int userId, QString profileName)
{
	this->profileId = profileId;
	this->userId = userId;
	this->profileName = profileName;
}
/** @brief Getter for Profile ID. */
int Profile::getProfileId() const
{
	return profileId;
}
/** @brief Getter for User ID. */
int Profile::getUserId() const
{
	return userId;
}
/** @brief Getter for Profile Name. */
QString Profile::getProfileName() const
{
	return profileName;
}