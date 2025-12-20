#include "Controller/BaseController.h"

int BaseController::s_currentUserId = 0;//namespace?
int BaseController::s_currentProfileId = 0;//namespace?

//Setters
void BaseController::setProfileId(int profileId)
{
	s_currentProfileId = profileId;
}

void BaseController::setUserId(int userId)
{
	s_currentUserId = userId;
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

//Getters
int BaseController::getProfileId()
{
	return s_currentProfileId;
}

int BaseController::getUserId()
{
	return s_currentUserId;
}

QString BaseController::getFilteringText()
{
	return filteringText;
}

int BaseController::getSelectedColumnId()
{
	return selectedColumnId;
}

Qt::SortOrder BaseController::getLastSortingOrder()
{
	return lastSortingOrder;
}

