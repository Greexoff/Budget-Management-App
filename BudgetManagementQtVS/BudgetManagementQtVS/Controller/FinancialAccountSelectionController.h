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

class FinancialAccountController : public BaseController
{
    Q_OBJECT

public:
    FinancialAccountController(FinancialAccountRepository& repoRef, QObject* parent = nullptr);
    void run();
    QWidget* getView();

    void showAccounts();

signals:
    void financialAccountDataChanged();

public slots:
    void setupFinancialAccountWindow();

    void handleFinancialAccountAddRequest(const QString& name, const QString& type, double balance);
    void handleFinancialAccountDeleteRequest(int id);
    void handleFinancialAccountEditRequest(int id, const QString& name, const QString& type, double balance);
    void handleFinancialAccountFilteringRequest(const QString& searchText);
    void handleSortingRequest(int columnId);

private:
    QPointer<FinancialAccountSelectionView> fAccountView; 
    FinancialAccountRepository& financialAccountRepository;

    int selectedFinancialAccountIdForTransaction = 1;

    void refreshTable();
    QVector<FinancialAccount> executeFilteringFinancialAccount(const QVector<FinancialAccount> allAccounts);
    void executeSortingFinancialAccount(QVector<FinancialAccount>& allAccounts) const;
};