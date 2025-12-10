#pragma once
#include <QDialog>
#include <Model/User.h>
#include <Model/Profile.h>

namespace Ui { class ProfileDialog; }

/**
 * @class ProfileDialog
 * @brief Dialog for selecting, adding, and removing budget profiles
 *
 * Users can have multiple budget profiles (e.g., personal, family, business).
 * This dialog allows selection of an existing profile or creation/deletion
 * of profiles.
 */
class ProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProfileDialog(QWidget* parent = nullptr);
    ~ProfileDialog();

    /**
     * @brief Populates the dialog with user's profiles
     * @param profiles List of Profile objects belonging to the user
     */
    void setProfiles(const QVector<Profile>& profiles);
public slots:
    void showProfileMessage(QString header, QString message, QString messageType);
signals:
    /**
     * @brief Emitted when user selects a profile
     * @param profileId ID of the selected profile
     */
    void profileSelected(int profileId);

    /**
     * @brief Emitted when user requests to add a new profile
     * @param profileName Name for the new profile
     */
    void addProfileRequested(const QString& profileName);

    /**
     * @brief Emitted when user requests to remove a profile
     * @param profileId ID of the profile to remove
     */
    void removeProfileRequested(int profileId);

    void editProfileRequested(int profileId, const QString& newName);

private slots:
    void onButtonSelectClicked();
    void onButtonAddClicked();
    void onButtonRemoveClicked();
    void onButtonCancelClicked();
    void onButtonEditClicked();

private:
    Ui::ProfileDialog* ui;          ///< Pointer to UI elements
    QVector<Profile> profilesId;    ///< Local copy of user's profiles

    /**
     * @brief Sets up connections for UI buttons
     */
    void setupConnections();    
};
