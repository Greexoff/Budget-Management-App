#include "Controller/DashboardController.h"

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
DashboardController::~DashboardController()
{
    if (dashboardView)
    {
        dashboardView->close();
    }
}

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

void DashboardController::setupConnections()
{
    connect(dashboardView, &DashboardView::pageChangeRequested,
        this, &DashboardController::handlePageChange);
}

void DashboardController::run() {
    if (dashboardView) {
        dashboardView->show();
        dashboardView->navigateToPage(DashboardView::TransactionPage);
    }

}
