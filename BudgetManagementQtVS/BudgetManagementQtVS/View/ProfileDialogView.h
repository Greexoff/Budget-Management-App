/**
 * @file ProfileDialogView.h
 * @brief Header file for the Profile Selection Dialog.
 */
#pragma once
#include <QDialog>
#include <Model/User.h>
#include <Model/Profile.h>

namespace Ui { class ProfileDialog; }

/**
 * @class ProfileDialog
 * @brief Handles profile selection, creation, deletion, and user management.
 */
class ProfileDialog : public QDialog
{
    Q_OBJECT
public:
    /** @brief Constructs the ProfileDialog. */
    explicit ProfileDialog(QWidget* parent = nullptr);
    /** @brief Destructor. */
    ~ProfileDialog();

    /**
     * @brief Populates the list of profiles.
     * @param profiles List of profile objects.
     */
    void setProfiles(const QVector<Profile>& profiles);
public slots:
    /**
     * @brief Shows a message box.
     * @param header Title.
     * @param message Content.
     * @param messageType Type ("error" etc.).
     */
    void showProfileMessage(QString header, QString message, QString messageType);
signals:
    /**
     * @brief Signal emitted when a profile is selected.
     * @param profileId ID of the selected profile.
     */
    void profileSelected(int profileId);

    /**
     * @brief Signal emitted to create a new profile.
     * @param profileName Name of the new profile.
     */
    void addProfileRequested(const QString& profileName);

    /**
     * @brief Signal emitted to remove a profile.
     * @param profileId ID to remove.
     */
    void removeProfileRequested(int profileId);

    /**
     * @brief Signal emitted to edit a profile.
     * @param profileId ID to edit.
     * @param newName New name.
     */
    void editProfileRequested(int profileId, const QString& newName);
    /** @brief Emitted to log out the user. */
    void logoutRequested();
    /** @brief Emitted to export user data. */
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
