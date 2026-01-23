/**
 * @file TransactionWindowView.h
 * @brief Header file for the Transaction Window View.
 */
#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QMap>
#include <QHeaderView>
#include <QMessageBox>

 /**
  * @class TransactionWindow
  * @brief Main view for listing transactions and managing the budget.
  */
class TransactionWindow : public QWidget
{
    Q_OBJECT

public:
    /** @brief Constructs the TransactionWindow. */
    explicit TransactionWindow(QWidget* parent = nullptr);

    /**
     * @brief Populates the transaction table model.
     * @param rows Vector of string lists containing transaction details.
     */
    void setTransactionTabHeaders(const QVector<QStringList>& rows) const;

    /** @brief Returns ID of selected transaction. */
    int getSelectedTransactionId() const;

    /**
     * @brief Updates budget label and progress bar.
     * @param limit Total budget limit.
     * @param spent Amount spent.
     */
    void updateBudgetDisplay(double limit, double spent) const;

    /** @brief Clears the search bar. */
    void clearSearchEdit() const;

    /** @brief Shows transaction related messages. */
    void showTransactionMessage(const QString& header, const QString& message, const QString& messageType);

signals:
    void addTransactionRequest();
    void deleteTransactionRequest();
    void editTransactionRequest();
    void editBudgetRequest();
    void columnSortRequest(int columnId);
    void searchTextRequest(const QString& searchText);

private slots:
    void onButtonAddClicked() { emit addTransactionRequest(); }
    void onButtonEditClicked() { emit editTransactionRequest(); }
    void onButtonDeleteClicked() { emit deleteTransactionRequest(); }
    void onButtonBudgetClicked() { emit editBudgetRequest(); }

private:
    QMap<QString, QPushButton*> actionButtons;
    QPushButton* createButton(const QString& text, const QString& objName, void (TransactionWindow::* slot)());
    QLayout* createHeaderSection();

    QStandardItemModel* tableModel;
    QLineEdit* searchEdit;
    QProgressBar* budgetProgressBar;
    QLabel* budgetLabel;
    QTableView* transactionTable;

    QWidget* createBudgetSection();
    QLayout* createActionSection();
    void setupUI();
    void setupStyle();
    void setupConnections();
    void initializeTable() const;
};