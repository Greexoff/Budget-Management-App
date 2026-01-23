/**
 * @file DashboardController.cpp
 * @brief Implementation of the Dashboard Controller.
 */
#include "Controller/DashboardController.h"

 /**
  * @brief Constructor. Creates the DashboardView and all sub-controllers (Transaction, Category, etc.).
  */
DashboardController::DashboardController(ProfilesRepository& profileRepo, TransactionRepository& transactionRepo,
    CategoryRepository& categoryRepo, FinancialAccountRepository& fAccRepo, QObject* parent)
{
    dashboardView = new DashboardView();

    transactionController = new TransactionController(transactionRepo, categoryRepo, fAccRepo, profileRepo);
    categoryController = new CategoryController(categoryRepo);
    financialAccountController = new FinancialAccountController(fAccRepo);
    chartsController = new ChartsController(transactionRepo, categoryRepo);
    if (dashboardView) {
        dashboardView->setAttribute(Qt::WA_DeleteOnClose);
    	connect(dashboardView, &DashboardView::backToProfileRequested, this, &DashboardController::logoutRequested);
        setupConnections();
    }
}
/** @brief Destructor closing the view. */
DashboardController::~DashboardController()
{
    if (dashboardView)
    {
        dashboardView->close();
    }
}
/**
 * @brief Switches the central widget based on the selected page enum.
 * Calls run() on the target sub-controller before displaying its view.
 */
void DashboardController::handlePageChange(DashboardView::SelectedPage page) const
{
    switch (page) {
    case DashboardView::TransactionPage:
        if (transactionController) {
            transactionController->run();
            dashboardView->replaceCurrentWidget(page, transactionController->getView());
        }
        break;
    case DashboardView::CategoryPage:
        if (categoryController) {
            categoryController->run();
            dashboardView->replaceCurrentWidget(page, categoryController->getView());
        }
        break;
    case DashboardView::FinancialAccountPage:
        if (financialAccountController)
        {
            financialAccountController->run();
            dashboardView->replaceCurrentWidget(page, financialAccountController->getView());
        }
        break;
    case DashboardView::ChartsPage:
        if (chartsController)
        {
            chartsController->run();
            dashboardView->replaceCurrentWidget(page, chartsController->getView());
        }
        break;
    }
}
/** @brief Connects view signals to controller slots. */
void DashboardController::setupConnections()
{
    connect(dashboardView, &DashboardView::pageChangeRequested,
        this, &DashboardController::handlePageChange);
}
/** @brief Shows the dashboard and defaults to Transaction page. */
void DashboardController::run() {
    if (dashboardView) {
        dashboardView->show();
        dashboardView->navigateToPage(DashboardView::TransactionPage);
    }

}
