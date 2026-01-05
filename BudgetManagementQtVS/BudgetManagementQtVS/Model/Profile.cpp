#include "Model/Profile.h"

Profile::Profile(int profileId, int userId, QString profileName)
{
	this->profileId = profileId;
	this->userId = userId;
	this->profileName = profileName;
}

int Profile::getProfileId() const
{
	return profileId;
}

int Profile::getUserId() const
{
	return userId;
}

QString Profile::getProfileName() const
{
	return profileName;
}