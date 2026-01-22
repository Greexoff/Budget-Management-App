#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFrame>

class TransactionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionWindow(QWidget* parent = nullptr);

    void setTransactionTabHeaders(const QVector<QStringList>& rows) const;
    int getSelectedTransactionId() const;
    void updateBudgetDisplay(double limit, double spent) const;
    void clearSearchEdit() const;
    void showTransactionMessage(const QString& header, const QString& message, const QString& messageType);

signals:
    void addTransactionRequest();
    void deleteTransactionRequest();
    void editTransactionRequest();
    void editBudgetRequest();
    void columnSortRequest(int columnId);
    void searchTextRequest(const QString& searchText);
    void backToProfileRequested();

private slots:
    void onButtonAddTransactionClicked() { emit addTransactionRequest(); }
    void onButtonDeleteTransactionClicked() { emit deleteTransactionRequest(); }
    void onButtonEditTransactionClicked() { emit editTransactionRequest(); }
    void onButtonEditBudgetClicked() { emit editBudgetRequest(); }
    void onSearchTextChanged(const QString& text) { emit searchTextRequest(text); }
    void onColumnHeaderClicked(int columnId) { emit columnSortRequest(columnId); }

private:
    void setupUI();
    void setupStyle();
    void setupConnections();
    void initializeTransactionTable() const;

    QStandardItemModel* tableModel;
    QLineEdit* searchEdit;
    QProgressBar* budgetProgressBar;
    QLabel* budgetLabel;
    QPushButton* btnEditBudget;
    QPushButton* btnAddTransaction;
    QPushButton* btnEditTransaction;
    QPushButton* btnDeleteTransaction;
    QTableView* transactionTable;
};