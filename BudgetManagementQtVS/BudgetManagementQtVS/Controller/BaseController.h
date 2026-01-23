/**
 * @file BaseController.h
 * @brief Header file for the Base Controller abstract class.
 */
#pragma once
#include <QObject>
#include <QVector>
#include <QString>

 /**
  * @class BaseController
  * @brief Abstract base class providing common functionality for all controllers.
  * Holds static session state (User ID, Profile ID) and helper methods for filtering/sorting.
  */
class BaseController : public QObject
{
	Q_OBJECT
public:
	explicit BaseController(QObject* parent = nullptr) : QObject(parent) {}
	~BaseController() override = default;
	/** @brief Pure virtual method to start the controller's logic. */
	virtual void run() = 0;
	// Static Session Management
	static void setProfileId(int profileId);
	static void setUserId(int userId);
	static int getProfileId();
	static int getUserId();
protected:
	// Filtering & Sorting Helpers
	QString getFilteringText();
	void setFilteringText(const QString& searchText);

	int getSelectedColumnId() const;
	void setSelectedColumnId(int columnId);

	Qt::SortOrder getLastSortingOrder() const;
	void setLastSortingOrder();

    /**
     * @brief Generic template method to filter a vector of items.
     * @tparam T Type of items in the vector.
     * @tparam P Predicate function/lambda type.
     * @param allItems The source vector.
     * @param match The filtering predicate.
     * @return A filtered vector of items.
     */
    template<typename T, typename P>
    QVector<T> executeFiltering(const QVector<T>& allItems, P match) {
        if (getFilteringText().isEmpty()) {
            return allItems;
        }

        QVector<T> filteredItems;
        std::copy_if(allItems.begin(), allItems.end(), std::back_inserter(filteredItems), match);
        return filteredItems;
    }

    /**
     * @brief Generic template method to sort a vector of items.
     * @tparam T Type of items.
     * @tparam C Comparator function/lambda type.
     * @param itemsToSort The vector to sort (in-place).
     * @param comp The comparison function.
     */
    template<typename T, typename C>
    void executeSorting(QVector<T>& itemsToSort, C comp) {
        Qt::SortOrder order = getLastSortingOrder();

        std::sort(itemsToSort.begin(), itemsToSort.end(), [comp, order](const T& a, const T& b) {
            if (order == Qt::AscendingOrder) {
                return comp(a, b);
            }
                return comp(b, a);
            });
    }

private:
	static int s_currentProfileId;
	static int s_currentUserId;
	static bool s_mainWindowInitialized;
	QString filteringText = "";
	int selectedColumnId = -1;
	Qt::SortOrder lastSortingOrder = Qt::SortOrder::AscendingOrder;
};