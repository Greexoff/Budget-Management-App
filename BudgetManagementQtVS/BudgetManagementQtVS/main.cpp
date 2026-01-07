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
#include "View/ChartsDialogView.h"

#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/UserRepository.h>
#include <Model/Repositories/ProfileRepository.h>
#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/FinancialAccountRepository.h>

#include <QtWidgets/QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    DatabaseManager::instance();

    UserRepository userRepository;
    ProfilesRepository profileRepository;
    TransactionRepository transactionRepository;
    CategoryRepository categoryRepository;
    FinancialAccountRepository financialAccountRepository;

    LoginDialog loginDialog;
    ProfileDialog profileDialog;
    TransactionWindow transactionWindow;
    CategorySelectionView categoryDialog;
    FinancialAccountSelectionView financialAccountDialog;
    ChartsDialogView chartsDialogView;

    UserController userController(loginDialog, userRepository, nullptr);
    ProfileController profileController(profileDialog, profileRepository, nullptr);

    TransactionController transactionController(
        transactionWindow,
        transactionRepository,
        categoryRepository,
        financialAccountRepository,
        profileRepository,
        nullptr
    );

    CategoryController categoryController(categoryDialog, categoryRepository, nullptr);
    FinancialAccountController financialAccountController(financialAccountDialog, financialAccountRepository, nullptr);

    ControllerManager controllerManager(userController, profileController, transactionController, categoryController, financialAccountController);

    userController.run();

    return app.exec();
}
