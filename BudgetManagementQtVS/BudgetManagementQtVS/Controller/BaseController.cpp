/**
 * @file BaseController.cpp
 * @brief Implementation of the Base Controller.
 */
#include "Controller/BaseController.h"

int BaseController::s_currentUserId = -1;
int BaseController::s_currentProfileId = -1;

/** @brief Sets the Profile ID. */
void BaseController::setProfileId(int profileId)
{
	s_currentProfileId = profileId;
}
/** @brief Sets the User ID. */
void BaseController::setUserId(int userId)
{
	s_currentUserId = userId;
}
/** @brief Sets the current text used for filtering lists. */
void BaseController::setFilteringText(const QString& searchText)
{
	filteringText = searchText;
}
/**
 * @brief Sets the column ID for sorting. Toggles sort order if the same column is selected twice.
 */
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
/** @brief Toggles the sort order between Ascending and Descending. */
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

int BaseController::getSelectedColumnId() const
{
	return selectedColumnId;
}

Qt::SortOrder BaseController::getLastSortingOrder() const
{
	return lastSortingOrder;
}

