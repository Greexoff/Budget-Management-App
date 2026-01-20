#include "Controller/UserController.h"
#include "Controller/ProfileController.h"
#include "Controller/TransactionController.h"
#include "Controller/CategorySelectionController.h"
#include "Controller/ControllerManager.h"
#include "Controller/FinancialAccountSelectionController.h"
#include "Controller/ChartsController.h"

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

    CategorySelectionView categoryView;
    transactionWindow.setCategoryView(&categoryView);

    FinancialAccountSelectionView financialAccountView;
    transactionWindow.setAccountView(&financialAccountView);

    ChartsView chartsView;
    transactionWindow.setChartsView(&chartsView);

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

    CategoryController categoryController(categoryView, categoryRepository, nullptr);
    FinancialAccountController financialAccountController(financialAccountView, financialAccountRepository, nullptr);
	ChartsController chartsController(transactionWindow, chartsView, transactionRepository, categoryRepository, nullptr);

    ControllerManager controllerManager(userController, profileController, transactionController, categoryController, financialAccountController, chartsController);

    userController.run();

    return app.exec();
}
