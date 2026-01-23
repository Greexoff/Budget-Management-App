#pragma once
#include <QObject>
#include <QVector>
#include <QString>


class BaseController : public QObject
{
	Q_OBJECT
public:
	explicit BaseController(QObject* parent = nullptr) : QObject(parent) {}
	~BaseController() override = default;

	virtual void run() = 0;
	static void setProfileId(int profileId);
	static void setUserId(int userId);
	static int getProfileId();
	static int getUserId();
protected:

	QString getFilteringText();
	void setFilteringText(const QString& searchText);

	int getSelectedColumnId() const;
	void setSelectedColumnId(int columnId);

	Qt::SortOrder getLastSortingOrder() const;
	void setLastSortingOrder();

    template<typename T, typename P>
    QVector<T> executeFiltering(const QVector<T>& allItems, P match) {
        if (getFilteringText().isEmpty()) {
            return allItems;
        }

        QVector<T> filteredItems;
        std::copy_if(allItems.begin(), allItems.end(), std::back_inserter(filteredItems), match);
        return filteredItems;
    }

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