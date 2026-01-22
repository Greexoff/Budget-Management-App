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

private slots:
    void onButtonAddClicked() { emit addTransactionRequest(); }
    void onButtonEditClicked() { emit editTransactionRequest(); }
    void onButtonDeleteClicked() { emit deleteTransactionRequest(); }
    void onButtonBudgetClicked() { emit editBudgetRequest(); }

private:
    QMap<QString, QPushButton*> actionButtons;
    QPushButton* createButton(const QString& text, const QString& objName, void (TransactionWindow::* slot)());

    QStandardItemModel* tableModel;
    QLineEdit* searchEdit;
    QProgressBar* budgetProgressBar;
    QLabel* budgetLabel;
    QTableView* transactionTable;

    void setupUI();
    void setupStyle();
    void setupConnections();
    void initializeTable() const;
};