#pragma once
#include <QDialog>
#include <Model/User.h>
#include <Model/Profile.h>

namespace Ui { class ProfileDialog; }


class ProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProfileDialog(QWidget* parent = nullptr);
    ~ProfileDialog();

    void setProfiles(const QVector<Profile>& profiles);
public slots:
    void showProfileMessage(QString header, QString message, QString messageType);
signals:

    void profileSelected(int profileId);

    void addProfileRequested(const QString& profileName);

    void removeProfileRequested(int profileId);

    void editProfileRequested(int profileId, const QString& newName);

    void logoutRequested();

    void exportDataRequested();

private slots:
    void onButtonSelectClicked();
    void onButtonAddClicked();
    void onButtonRemoveClicked();
    void onButtonCancelClicked();
    void onButtonEditClicked();
    void onButtonLogoutClicked();
    void onButtonExportClicked();
private:
    Ui::ProfileDialog* ui;          
    QVector<Profile> profilesId;    

    void setupConnections();
    void setupStyle();
};
