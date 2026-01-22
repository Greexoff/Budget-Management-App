#pragma once
#include "Controller/BaseController.h"
#include "View/DashboardView.h"
#include <QPointer>

#include "Controller/CategorySelectionController.h"
#include "Controller/ChartsController.h"
#include "Controller/FinancialAccountSelectionController.h"
#include "Controller/TransactionController.h"
#include "Model/Repositories/UserRepository.h"

class DashboardController : public BaseController {
    Q_OBJECT

public:
    explicit DashboardController(ProfilesRepository& profileRepo, TransactionRepository& transactionRepo, CategoryRepository& categoryRepo, FinancialAccountRepository& fAccRepo, QObject* parent = nullptr);
    void run() override;
    DashboardController(QObject* parent);
    ~DashboardController() override;
signals:
    void logoutRequested();

private slots:
    void handlePageChange(DashboardView::SelectedPage page) const;
private:
    QPointer<DashboardView> dashboardView;
    QPointer<TransactionController> transactionController;
    QPointer<CategoryController> categoryController;
    QPointer<FinancialAccountController> financialAccountController;
    QPointer<ChartsController> chartsController;

    void setupConnections();

};
