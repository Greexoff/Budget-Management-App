#pragma once

#include "Model/Transaction.h"
#include "Model/Repositories.h"
#include "Model/User&Profiles.h"
#include "Model/Category.h"
#include "Model/TransactionBuilder.h"

#include "View/TransactionWindowView.h"
#include "View/LoginDialogView.h"
#include "View/ProfileDialogView.h"
#include "View/CategorySelectionView.h"

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
     * @brief Constructs the controller with optional parent
     * @param parent Parent QObject (optional)
     */
    Controller(
        LoginDialog& loginDialogRef,
        ProfileDialog& profileDialogRef,
        CategorySelectionView& categorySelectionViewRef,
        TransactionWindow& transactionWindowViewRef,
        UserRepository& userRepositoryRef,
        ProfilesRepository& profileRepositoryRef,
        TransactionRepository& transactionRepositoryRef,
        CategoryRepository& categoryRepositoryRef,
        QObject* parent = nullptr);
    
    /**
     * @brief Starts the application by showing the login screen
     */
    void run();

private slots:
    // Authentication handlers
    void handleLoginRequest(const QString& username, const QString& password);
    void handleRegisterRequest(const QString& username, const QString& password);

    // Profile management handlers
    void handleProfileSelection(int profileId);
    void handleAddProfileRequest(const QString& name);
    void handleRemoveProfileRequest(int profileId);

    // Transaction management handlers
    void handleAddTransactionRequest();
    void handleDeleteTransactionRequest();

    // Category management handlers
    void handleCategorySelection(int categoryId);
    void handleAddCategoryRequest(const QString& categoryName);
    void handleDeleteCategoryRequest(int categoryId);

    /**
     * @brief Shows category management dialog
     */
    void handleShowCategorySelectionRequest();
private:
    // View instances
    LoginDialog& loginDialog;
    ProfileDialog& profileDialog;
    TransactionWindow& TransactionWindowView;
    CategorySelectionView& categoryDialog;

    // Repository instances
    UserRepository& userRepository;
    ProfilesRepository& profilesRepository;
    TransactionRepository& transactionRepository;
    CategoryRepository& categoryRepository;

    // Application state
    int currentUserId = -1;                     ///< ID of currently logged-in user
    int currentProfileId = -1;                  ///< ID of currently selected profile
    bool mainWindowInitialized = false;         ///< Flag indicating if main window is set up
    int selectedCategoryIdForTransaction = -1;  ///< ID of category selected for transaction

    // Helper methods
    void showProfilesForCurrentUser();
    void refreshTransactionsView();
    void showCategoryDialog(bool withSelectButton);
    int getCategoryIdFromInput();
    void refreshCategoryDialogList();
};

 