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

void BaseController::setSelectedColumnId(int columnId)
{
	if (columnId == selectedColumnId)
	{
		setLastSortingOrder();
	}
	else
	{
		selectedColumnId = columnId;
		lastSortingOrder = Qt::SortOrder::AscendingOrder;
	}
}

int BaseController::getSelectedColumnId()
{
	return selectedColumnId;
}

Qt::SortOrder BaseController::getLastSortingOrder()
{
	return lastSortingOrder;
}

void BaseController::setLastSortingOrder()
{
	if (lastSortingOrder == Qt::SortOrder::AscendingOrder)
	{
		lastSortingOrder = Qt::SortOrder::DescendingOrder;
	}
	else
	{
		lastSortingOrder = Qt::SortOrder::AscendingOrder;
	}
}
