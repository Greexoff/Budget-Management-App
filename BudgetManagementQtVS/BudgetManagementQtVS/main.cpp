#include "Controller/Controller.h"
#include "Controller/CategorySelectionController.h"

#include "View/TransactionWindowView.h"
#include "View/CategorySelectionView.h"
#include "View/LoginDialogView.h"
#include "View/ProfileDialogView.h"

#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/UserRepository.h>
#include <Model/Repositories/ProfileRepository.h>
#include <Model/Repositories/TransactionRepository.h>

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

    // Create view instances for presentation layer
    LoginDialog loginDialog;
    ProfileDialog profileDialog;
    CategorySelectionView categoryDialog;
    TransactionWindow transactionWindowView;

    CategoryController categoryController(categoryDialog, categoryRepository, nullptr);
    // Create controller which manages application flow
    Controller controller(loginDialog, profileDialog, transactionWindowView, userRepository, profileRepository, transactionRepository, categoryController, nullptr);

    // Start the application by showing login screen
    controller.run();

    // Enter Qt event loop (application runs until quit)
    return app.exec();
}
