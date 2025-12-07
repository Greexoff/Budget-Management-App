#pragma once

#include "Model/Transaction.h"
#include "Model/Repositories.h"
#include "Model/User&Profiles.h"
#include "Model/Category.h"

#include "View/MainWindowView.h"
#include "View/LoginDialogView.h"
#include "View/ProfileDialogView.h"
#include "View/CategorySelectionView.h"

#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDate>

class MainWindow;

/**
 * @brief Main controller coordinating application flow and business logic
 * This class manages user interactions, coordinates between views and models,
 * and handles application state transitions including login, profile selection,
 * and transaction management.
 */
class TransactionController : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the controller with optional parent
     * @param parent Parent QObject (optional)
     */
    explicit TransactionController(QObject* parent = nullptr);
    
    /**
     * @brief Starts the application by showing the login screen
     */
    void run();

private slots:
    // Authentication handlers
    void handleLoginRequested(const QString& username, const QString& password);
    void handleRegisterRequested(const QString& username, const QString& password);

    // Profile management handlers
    void handleProfileSelected(int profileId);
    void handleAddProfileRequested(const QString& name);
    void handleRemoveProfileRequested(int profileId);

    // Transaction management handlers
    void handleAddTransactionRequested();
    void handleDeleteTransactionRequested();

    // Category management handlers
    void handleCategorySelected(int categoryId);
    void handleAddCategoryRequested(const QString& categoryName);
    void handleDeleteCategoryRequested(int categoryId);

    /**
     * @brief Shows category management dialog
     */
    void showCategorySelectionDialog();
private:
    // View instances
    LoginDialog loginDialog;
    ProfileDialog profileDialog;
    MainWindow mainWindowView;
    CategorySelectionView categoryDialog;

    // Repository instances
    UserRepository userRepository;
    ProfilesRepository profilesRepository;
    TransactionRepository transactionRepository;
    CategoryRepository categoryRepository;

    // Application state
    int currentUserId = -1;                     ///< ID of currently logged-in user
    int currentProfileId = -1;                  ///< ID of currently selected profile
    bool mainWindowInitialized = false;         ///< Flag indicating if main window is set up
    int selectedCategoryIdForTransaction = -1;  ///< ID of category selected for transaction

    // Helper methods
    void showProfilesForCurrentUser();
    void refreshTransactionsView();
    void openCategoryDialog(bool withSelectButton);
    int askUserForCategoryId();
    void refreshCategoryDialogList();
};

 