/**
 * @file ProfileController.h
 * @brief Header file for the Profile Controller.
 */
#pragma once
#include "Controller/BaseController.h"

#include "Model/Repositories/ProfileRepository.h"
#include "Controller/DataController.h"

#include "View/ProfileDialogView.h"

#include <QPointer>

 /**
  * @class ProfileController
  * @brief Manages Profile selection, creation, and session management.
  */
class ProfileController : public BaseController
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the ProfileController.
     * @param profileRepositoryRef Reference to the profile repository.
     * @param parent Parent QObject.
     */
    ProfileController(ProfilesRepository& profileRepositoryRef, QObject* parent = nullptr);

    /** @brief Runs the controller. */
    void run() override;
signals:
    /** @brief Emitted when a profile is successfully selected. */
    void profileSelected();
    /** @brief Emitted when user logs out. */
    void logout();

public slots:
    /** @brief Refreshes the list of profiles for the currently logged-in user. */
    void refreshProfilesForCurrentUser() const;

private:
    QPointer<ProfileDialog> profileDialog;

    ProfilesRepository& profileRepository;

    DataController* dataController;

    /**
     * @brief Handles the selection of a profile.
     * @param profileId The ID of the selected profile.
     */
    void handleProfileSelection(int profileId);

    /**
     * @brief Handles the request to add a new profile.
     * @param name The name of the new profile.
     */
    void handleAddProfileRequest(const QString& name) const;

    /**
     * @brief Handles the request to remove a profile.
     * @param profileId The ID of the profile to remove.
     */
    void handleRemoveProfileRequest(int profileId) const;

    /**
     * @brief Handles the request to edit a profile's name.
     * @param profileId The ID of the profile to edit.
     * @param newName The new name for the profile.
     */
    void handleEditProfileRequest(int profileId, const QString& newName) const;

    void handleLogoutRequest();

    void handleExportDataRequest() const;
};

