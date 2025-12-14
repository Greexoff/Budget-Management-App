#pragma once
#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDate>

class BaseController : public QObject
{
	Q_OBJECT
public:
	explicit BaseController(QObject* parent = nullptr): QObject(parent) {}
protected:
	void setProfileId(int profileId);
	int getProfileId();

	void setUserId(int userId);
	int getUserId();

	void setMainWindowInitializedAttribute(bool value);
	bool getMainWindowInitializedAttribute();

	QString getFilteringText();
	void setFilteringText(const QString& searchText);

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

private:
	static int s_currentProfileId;
	static int s_currentUserId;
	static bool s_mainWindowInitialized;
	QString filteringText = "";
};