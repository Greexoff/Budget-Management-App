#pragma once

#include "Model/Transaction.h"

#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/UserRepository.h>
#include <Model/Repositories/ProfileRepository.h>
#include <Model/Repositories/TransactionRepository.h>

#include <Model/User.h>
#include <Model/Profile.h>
#include "Model/TransactionBuilder.h"

#include "Controller/CategorySelectionController.h"

#include "View/TransactionWindowView.h"
#include "View/LoginDialogView.h"
#include "View/ProfileDialogView.h"

#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDate>


/**
 * @brief Main controller coordinating application flow and business logic
 * This class manages user interactions, coordinates between views and models,
 * and handles application state transitions including login, profile selection,
 * and transaction management.
 */

class Controller : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the controller with dependency injection
     *
     * @param loginDialogRef Reference to the login dialog view
     * @param profileDialogRef Reference to the profile dialog view
     * @param categorySelectionViewRef Reference to the category selection view
     * @param transactionWindowViewRef Reference to the transaction window view
     * @param userRepositoryRef Reference to the user repository
     * @param profileRepositoryRef Reference to the profile repository
     * @param transactionRepositoryRef Reference to the transaction repository
     * @param categoryRepositoryRef Reference to the category repository
     * @param parent Parent QObject
     */
    Controller(
        LoginDialog& loginDialogRef,
        ProfileDialog& profileDialogRef,
        TransactionWindow& transactionWindowViewRef,
        UserRepository& userRepositoryRef,
        ProfilesRepository& profileRepositoryRef,
        TransactionRepository& transactionRepositoryRef,
        CategoryController& categoryControllerRef,
        QObject* parent = nullptr);
    
    /**
     * @brief Starts the application by showing the login screen
     */
    void run();
public slots:
    void handleCategoriesDataChangeRequest();
private slots:
    // Authentication handlers
     /**
     * @brief Handles user login request
     * @param username User's login name
     * @param password User's password
     */
    void handleLoginRequest(const QString& username, const QString& password);

    /**
     * @brief Handles new user registration request
     * @param username Desired username
     * @param password Desired password
     */
    void handleRegisterRequest(const QString& username, const QString& password);

    // Profile management handlers

    /**
     * @brief Handles profile selection
     * @param profileId ID of the selected profile
     */
    void handleProfileSelection(int profileId);

    /**
     * @brief Handles request to add a new profile
     * @param name Name for the new profile
     */
    void handleAddProfileRequest(const QString& name);

    /**
     * @brief Handles request to remove a profile
     * @param profileId ID of the profile to delete
     */
    void handleRemoveProfileRequest(int profileId);

    // Transaction management handlers

    /**
     * @brief Handles request to add a new transaction
     */
    void handleAddTransactionRequest();

    /**
     * @brief Handles request to delete a transaction
     */
    void handleDeleteTransactionRequest();


private:
    // View instances 
    LoginDialog& loginDialog;                   ///< Reference to the login dialog
    ProfileDialog& profileDialog;               ///< Reference to the profile dialog
    TransactionWindow& TransactionWindowView;   ///< Reference to the transaction window

    //Controller instances
    CategoryController& categoryController;

    // Repository instances 
    UserRepository& userRepository;                 ///< Reference to the user repository
    ProfilesRepository& profilesRepository;         ///< Reference to the profiles repository
    TransactionRepository& transactionRepository;   ///< Reference to the transaction repository

    // Application state
    int currentUserId = -1;                     ///< ID of currently logged-in user
    int currentProfileId = -1;                  ///< ID of currently selected profile
    bool mainWindowInitialized = false;         ///< Flag indicating if main window is set up

    // Helper methods

    /**
     * @brief Displays profiles associated with the current user
     */
    void showProfilesForCurrentUser();

    /**
     * @brief Refreshes the transaction display in the main window
     */
    void refreshTransactionsView();

    void handleShowCategoriesRequestFromView();
};

 