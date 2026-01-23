/**
 * @file DashboardController.h
 * @brief Header file for the Dashboard Controller.
 */
#pragma once
#include "Controller/BaseController.h"
#include "View/DashboardView.h"
#include <QPointer>

#include "Controller/CategorySelectionController.h"
#include "Controller/ChartsController.h"
#include "Controller/FinancialAccountSelectionController.h"
#include "Controller/TransactionController.h"
#include "Model/Repositories/UserRepository.h"

 /**
  * @class DashboardController
  * @brief Manages the main dashboard layout and switching between sub-controllers (Transactions, Category, etc.).
  */
class DashboardController : public BaseController {
    Q_OBJECT

public:
    /** @brief Constructs the DashboardController and initializes sub-controllers. */
    explicit DashboardController(ProfilesRepository& profileRepo, TransactionRepository& transactionRepo, CategoryRepository& categoryRepo, FinancialAccountRepository& fAccRepo, QObject* parent = nullptr);

    /** @brief Starts the dashboard (shows view). */
    void run() override;

    ~DashboardController() override;
signals:
    /** @brief Emitted when user wants to log out from the dashboard. */
    void logoutRequested();

private slots:
    /** @brief Handles navigation requests from the side menu. */
    void handlePageChange(DashboardView::SelectedPage page) const;

private:
    QPointer<DashboardView> dashboardView;
    QPointer<TransactionController> transactionController;
    QPointer<CategoryController> categoryController;
    QPointer<FinancialAccountController> financialAccountController;
    QPointer<ChartsController> chartsController;

    void setupConnections();

};
