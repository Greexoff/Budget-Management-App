#include "Controller/BaseController.h"

void BaseController::setProfileId(int profileId)
{
	currentProfileId = profileId;
}
int BaseController::getProfileId()
{
	return currentProfileId;
}

void BaseController::setUserId(int userId)
{
	currentUserId = userId;
}
int BaseController::getUserId()
{
	return currentUserId;
}

void BaseController::setMainWindowInitializedAttribute(bool value)
{
	mainWindowInitialized = value;
}
bool BaseController::getMainWindowInitializedAttribute()
{
	return mainWindowInitialized;
}