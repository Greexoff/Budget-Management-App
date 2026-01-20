#pragma once
#include "View/CategorySelectionView.h"
#include "View/FinancialAccountSelectionView.h"
#include "View/ChartsDialogView.h"

#include <QMainWindow>
#include <QStandardItemModel>
#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QStackedWidget> 

class TransactionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TransactionWindow(QWidget* parent = nullptr);
    ~TransactionWindow();

    void setCategoryView(CategorySelectionView* view);
    void setAccountView(FinancialAccountSelectionView* view);
    void setChartsView(ChartsView* view);


    void navigateToPage(int pageIndex);

    void setTransactionTabHeaders(const QVector<QStringList>& rows);
    int getSelectedTransactionId() const;
    void updateBudgetDisplay(double limit, double spent);
    void clearSearchEdit();
    void showTransactionMessage(QString header, QString message, QString messageType);

signals:

    void backToProfileRequested();


    void addTransactionRequest();
    void deleteTransactionRequest();
    void editTransactionRequest();
    void editBudgetRequest();
    void columnSortRequest(int columnId);
    void searchTextRequest(const QString& searchText);

    void showChartsRequest();

private slots:
 
    void onButtonChangeProfileClicked();


    void onButtonAddTransactionClicked();
    void onButtonDeleteTransactionClicked();
    void onButtonEditTransactionClicked();
    void onButtonEditBudgetClicked();
    void onColumnHeaderClicked(int columnId);
    void searchTextChanged(const QString& text);

private:
    QWidget* centralWidget;
    QStackedWidget* stackedWidget;


    QPushButton* btnNavTransactions;
    QPushButton* btnNavCategories;
    QPushButton* btnNavAccounts;
    QPushButton* btnNavCharts;
    QPushButton* btnNavProfile;

    CategorySelectionView* categoryView = nullptr;
    FinancialAccountSelectionView* accountView = nullptr;
    ChartsView* chartsView = nullptr;
    

    QWidget* pageTransactions;
    QStandardItemModel* TableModel;
    QLineEdit* searchEdit;
    QProgressBar* budgetProgressBar;
    QLabel* budgetLabel;
    QPushButton* btnEditBudget;
    QPushButton* btnAddTransaction;
    QPushButton* btnEditTransaction;
    QPushButton* btnDeleteTransaction;
    QTableView* transactionTable;


    void setupUI();
    void setupStyle();
    void setupConnections();
    void updateSidebarStyle(int activeIndex);

    QWidget* createTransactionsPage();
    void initializeTransactionTable();
};