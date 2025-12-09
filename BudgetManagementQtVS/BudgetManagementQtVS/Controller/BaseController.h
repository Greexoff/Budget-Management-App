#pragma once
#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDate>

class BaseController : public QObject
{
	Q_OBJECT
public:
	explicit BaseController(QObject* parent = nullptr) : QObject(parent), currentProfileId(0), currentUserId(0), mainWindowInitialized(false) {}
protected:
	void setProfileId(int profileId);
	int getProfileId();

	void setUserId(int userId);
	int getUserId();

	void setMainWindowInitializedAttribute(bool value);
	bool getMainWindowInitializedAttribute();

private:
	int currentProfileId;
	int currentUserId;
	bool mainWindowInitialized;
};
