/**
 * @file FinancialAccountSelectionController.h
 * @brief Header file for the Financial Account Controller.
 */
#pragma once

#include <Model/Repositories/FinancialAccountRepository.h>
#include <Model/FinancialAccount.h>
#include <Model/TransactionBuilder.h>

#include <Controller/BaseController.h>

#include <View/FinancialAccountSelectionView.h>

#include <QObject>
#include <QVector>
#include <QStringList>
#include <QPointer>

 /**
  * @class FinancialAccountController
  * @brief Manages logic for Financial Accounts (CRUD, Sort, Filter).
  */
class FinancialAccountController : public BaseController
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the controller.
     * @param repoRef Reference to the financial account repository.
     * @param parent Parent QObject.
     */
    FinancialAccountController(FinancialAccountRepository& repoRef, QObject* parent = nullptr);

    /** @brief Starts the controller logic (refreshes the table). */
    void run();

    /** @brief Returns the associated view widget. */
    QWidget* getView();

signals:
    /** @brief Emitted when financial account data has been modified (added, edited, deleted). */
    void financialAccountDataChanged();

public slots:
    /** @brief Initializes the window connections and initial data. */
    void setupFinancialAccountWindow();

    /**
     * @brief Handles the request to add a new financial account.
     * @param name The name of the new account.
     * @param type The type of the account (e.g., "Cash", "Bank").
     * @param balance The initial balance of the account.
     */
    void handleFinancialAccountAddRequest(const QString& name, const QString& type, double balance);

    /**
     * @brief Handles the request to delete a financial account.
     * @param id The ID of the account to delete.
     */
    void handleFinancialAccountDeleteRequest(int id);

    /**
     * @brief Handles the request to edit an existing financial account.
     * @param id The ID of the account to edit.
     * @param name The new name.
     * @param type The new type.
     * @param balance The new balance.
     */
    void handleFinancialAccountEditRequest(int id, const QString& name, const QString& type, double balance);

    /**
     * @brief Handles filtering requests from the search bar.
     * @param searchText The text to filter by.
     */
    void handleFinancialAccountFilteringRequest(const QString& searchText);

    /**
     * @brief Handles sorting requests from the table header.
     * @param columnId The index of the column to sort by.
     */
    void handleSortingRequest(int columnId);

private:
    QPointer<FinancialAccountSelectionView> fAccountView; 
    FinancialAccountRepository& financialAccountRepository;

    int selectedFinancialAccountIdForTransaction = 1;

    void refreshTable();
    QVector<FinancialAccount> executeFilteringFinancialAccount(const QVector<FinancialAccount> allAccounts);
    void executeSortingFinancialAccount(QVector<FinancialAccount>& allAccounts);
    void executeSortingFinancialAccount(QVector<FinancialAccount>& allAccounts) const;
};