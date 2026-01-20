#include <Controller/ControllerManager.h>
ControllerManager::ControllerManager(UserController& userControllerRef, ProfileController& profileControllerRef, TransactionController& transactionControllerRef, CategoryController& categoryControllerRef, FinancialAccountController& financialAccountControllerRef, ChartsController& chartsControllerRef, QObject* parent) : QObject(parent), userController(userControllerRef), profileController(profileControllerRef), transactionController(transactionControllerRef), categoryController(categoryControllerRef), financialAccountController(financialAccountControllerRef), chartsController(chartsControllerRef)
{
    
    connect(&userController, &UserController::userLoggedIn,
        &profileController, &ProfileController::refreshProfilesForCurrentUser);


    connect(&profileController, &ProfileController::profileSelected,
        &transactionController, &TransactionController::setupTransactionWindow);


    connect(&transactionController, &TransactionController::showCategories,
        &categoryController, &CategoryController::setupCategoryView);

	//connect(&transactionController, &TransactionController::showCharts,
		//&chartsController, &ChartsController::setupChartsWindow); // CHARTS!!


    connect(&transactionController, &TransactionController::showFinancialAccounts,
        &financialAccountController, &FinancialAccountController::setupFinancialAccountWindow);


    connect(&categoryController, &CategoryController::categoriesDataChanged,
        &transactionController, &TransactionController::handleDataChangeRequest);


    connect(&financialAccountController, &FinancialAccountController::financialAccountDataChanged,
        &transactionController, &TransactionController::handleDataChangeRequest);


    connect(&profileController, &ProfileController::logout,
        &userController, &UserController::run);


    connect(&transactionController, &TransactionController::returnToProfileView,
        &profileController, &ProfileController::refreshProfilesForCurrentUser);
}