#include "Controller/UserController.h"
#include "Controller/ProfileController.h"
#include "Controller/TransactionController.h"
#include "Controller/CategorySelectionController.h"
#include "Controller/ControllerManager.h"
#include "Controller/FinancialAccountSelectionController.h"

#include "View/TransactionWindowView.h"
#include "View/CategorySelectionView.h"
#include "View/LoginDialogView.h"
#include "View/ProfileDialogView.h"
#include "View/FinancialAccountSelectionView.h"

#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/UserRepository.h>
#include <Model/Repositories/ProfileRepository.h>
#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/FinancialAccountRepository.h>

#include <QtWidgets/QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

/**
 * @brief Application entry point
 *
 * Initializes the Qt application, sets up the database connection,
 * creates the main controller, and starts the application event loop.
 *
 * @param argc Command line argument count
 * @param argv Command line argument values
 * @return int Application exit code
 */
int main(int argc, char *argv[])
{
    // Initialize Qt application framework
    QApplication app(argc, argv);

    // Initialize database connection
    DatabaseManager::instance();

    // Create repository instances for data access layer
    UserRepository userRepository;
    ProfilesRepository profileRepository;
    TransactionRepository transactionRepository;
    CategoryRepository categoryRepository;
    FinancialAccountRepository financialAccountRepository;

    // Create view instances for presentation layer
    LoginDialog loginDialog;
    ProfileDialog profileDialog;
    TransactionWindow transactionWindow;
    CategorySelectionView categoryDialog;
    FinancialAccountSelectionView financialAccountDialog;

    UserController userController(loginDialog, userRepository, nullptr);
    ProfileController profileController(profileDialog, profileRepository, nullptr);
    TransactionController transactionController(transactionWindow, transactionRepository, categoryRepository, financialAccountRepository, nullptr);
    CategoryController categoryController(categoryDialog, categoryRepository, nullptr);
    FinancialAccountController financialAccountController(financialAccountDialog, financialAccountRepository, nullptr);


    ControllerManager controllerManager(userController, profileController, transactionController, categoryController, financialAccountController);

    userController.run();

    // Enter Qt event loop (application runs until quit)
    return app.exec();
}
