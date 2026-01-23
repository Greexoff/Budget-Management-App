/**
 * @file FinancialAccountSelectionView.h
 * @brief Header file for the Financial Account Selection View.
 */
#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>

 /**
  * @class FinancialAccountSelectionView
  * @brief Manages the display and CRUD operations for Financial Accounts.
  */
class FinancialAccountSelectionView : public QWidget
{
    Q_OBJECT

public:
    /** @brief Constructs the view. */
    explicit FinancialAccountSelectionView(QWidget* parent = nullptr);

    /**
     * @brief Populates the account table with data.
     * @param rows Vector of string lists containing account details.
     */
    void setAccountTabHeaders(const QVector<QStringList>& rows) const;

    /** @brief Returns the ID of the selected account. */
    int getSelectedAccountId() const;

    /**
     * @brief Shows a message box.
     * @param header Title.
     * @param message Content.
     * @param messageType Type ("error", "info").
     */
    void showMessage(QString header, QString message, QString messageType);

    /** @brief Returns text from the search bar. */
    QString getSearchText() const;

    /** @brief Triggers a view refresh. */
    void refreshView() { emit refreshRequest(); }

signals:
    /**
     * @brief Request to add a new account.
     * @param name Name of the account.
     * @param type Type (e.g., "Cash", "Bank").
     * @param balance Initial balance.
     */
    void addAccountRequest(const QString& name, const QString& type, double balance);
    /** @brief Signal to delete the selected account. */
    void deleteAccountRequest();
    /**
     * @brief Request to edit an account.
     * @param id Account ID.
     * @param name New name.
     * @param type New type.
     * @param balance New balance.
     */
    void editAccountRequest(int id, const QString& name, const QString& type, double balance);
    /**
     * @brief Request to filter accounts.
     * @param searchText Text to search.
     */
    void searchAccountRequest(const QString& searchText);
    /**
     * @brief Request to sort the table.
     * @param columnId Column index.
     */
    void columnSortRequest(int columnId);
    void refreshRequest();

private slots:
    void onButtonAddClicked();
    void onButtonEditClicked();
    void onButtonDeleteClicked();
    void onSearchTextChanged(const QString& text);
    void onHeaderClicked(int index);

private:
    QStandardItemModel* tableModel;
    QTableView* accountTable;
    QLineEdit* searchEdit;
    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnDelete;

    void setupUI();
    void setupStyle();
};