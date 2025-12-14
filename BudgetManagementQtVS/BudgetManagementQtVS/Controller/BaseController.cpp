#include "Controller/BaseController.h"

int BaseController::s_currentUserId = 0;
int BaseController::s_currentProfileId = 0;
bool BaseController::s_mainWindowInitialized = false;

	
void BaseController::setProfileId(int profileId)
{
	s_currentProfileId = profileId;
}
int BaseController::getProfileId()
{
	return s_currentProfileId;
}

void BaseController::setUserId(int userId)
{
	s_currentUserId = userId;
}
int BaseController::getUserId()
{
	return s_currentUserId;
}

void BaseController::setMainWindowInitializedAttribute(bool value)
{
	s_mainWindowInitialized = value;
}
bool BaseController::getMainWindowInitializedAttribute()
{
	return s_mainWindowInitialized;
}

QString BaseController::getFilteringText()
{
	return filteringText;
}

void BaseController::setFilteringText(const QString& searchText)
{
	filteringText = searchText;
}