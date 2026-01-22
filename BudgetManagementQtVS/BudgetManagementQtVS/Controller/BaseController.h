#pragma once
#include <QObject>
#include <QVector>
#include <QStringList>
#include <QString>
#include <QVariant>
#include <QDate>
#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/FinancialAccountRepository.h>

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

	int getSelectedColumnId();
	void setSelectedColumnId(int columnId);

	Qt::SortOrder getLastSortingOrder();
	void setLastSortingOrder();

	template<typename T, typename matchFound>
	QVector<T> executeFiltering(const QVector<T>& allItems, matchFound match)
	{
		if (filteringText.isEmpty())
		{
			return allItems;
		}
		QVector<T> filteredItems;

		for (const auto& item : allItems)
		{
			if (match(item))
			{
				filteredItems.append(item);
			}
		}
		return filteredItems;
	}

	template<typename T>
	QVector<T> executeSorting(QVector<T>& itemsToSort, int columnId, Qt::SortOrder order)
	{
		auto compareValues = [order](const auto& valA, const auto& valB) -> bool
			{
				if (order == Qt::AscendingOrder)
				{
					return valA < valB;
				}
				else
				{
					return valA > valB;
				}
			};
		if constexpr (std::is_same_v<T, Category>)
		{
			auto comparator = [columnId, compareValues](T& itemA, T& itemB) -> bool
				{
					switch (columnId)
					{
					case 0://Name
						return compareValues(itemA.getCategoryName().trimmed().toLower(), itemB.getCategoryName().trimmed().toLower());
						break;
					default:
						return compareValues(itemA.getCategoryId(), itemB.getCategoryId());
						break;
					}
				};
			std::sort(itemsToSort.begin(), itemsToSort.end(), comparator);
		}
		if constexpr (std::is_same_v<T, FinancialAccount>)
		{
			auto comparator = [columnId, compareValues](T& itemA, T& itemB) -> bool
				{
					switch (columnId)
					{
					case 0://Name
						return compareValues(itemA.getFinancialAccountName().trimmed().toLower(), itemB.getFinancialAccountName().trimmed().toLower());
						break;
					case 1://Type
						return compareValues(itemA.getFinancialAccountType().trimmed().toLower(), itemB.getFinancialAccountType().trimmed().toLower());
						break;
					case 2://Balance
						return compareValues(itemA.getFinancialAccountBalance(), itemB.getFinancialAccountBalance());
						break;
					default:
						return compareValues(itemA.getFinancialAccountId(), itemB.getFinancialAccountId());
						break;
					}
				};
			std::sort(itemsToSort.begin(), itemsToSort.end(), comparator);
		}
		return itemsToSort;
	}

private:
	static int s_currentProfileId;
	static int s_currentUserId;
	static bool s_mainWindowInitialized;
	QString filteringText = "";//Replace with namespace
	int selectedColumnId = -1;//Replace with namespace
	Qt::SortOrder lastSortingOrder = Qt::SortOrder::AscendingOrder;//Replace with namespace
};