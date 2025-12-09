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

private:
	static int s_currentProfileId;
	static int s_currentUserId;
	static bool s_mainWindowInitialized;
};
