/**
 * @file TransactionEditorDialogView.h
 * @brief Header file for the Transaction Editor (Add/Edit) Dialog.
 */
#pragma once

#include <QDialog>
#include <QInputDialog>
#include <ui_TransactionEditorDialogView.h>
#include <Model/Category.h>
#include <Model/FinancialAccount.h>
#include <Model/Transaction.h>

 /**
  * @class TransactionEditorDialogView
  * @brief Form dialog for creating or editing a transaction.
  */
class TransactionEditorDialogView : public QDialog {
	Q_OBJECT

public:
    /** @brief Constructs the dialog. */
    explicit TransactionEditorDialogView(QWidget* parent = nullptr);
    /** @brief Destructor. */
    ~TransactionEditorDialogView();

    /** @brief Populates the categories combo box. */
    void setCategories(const QVector<Category>& categories);
    /** @brief Populates the financial accounts combo box. */
    void setFinancialAccounts(const QVector<FinancialAccount>& financialAccounts);
    /** @brief Populates the transaction type combo box. */
    void setTransactionTypes();

    // Getters
    QString getName() const;
    double getAmount() const;
    QDate getDate() const;
    QString getDescription() const;
    QString getType() const;
    int getSelectedCategoryId() const;
    int getSelectedFinancialAccountId() const;

    // Setters (for Edit mode)
    /** @brief Sets the name field. @param name The transaction name. */
    void setName(const QString& name) const;

    /** @brief Sets the amount field. @param amount The amount. */
    void setAmount(double amount) const;

    /** @brief Sets the date field. @param date The date. */
    void setDate(const QDate& date) const;

    /** @brief Sets description. @param description The description. */
    void setDescription(const QString& description) const;

    /** @brief Sets transaction type. @param type "Income" or "Expense". */
    void setType(const QString& type);

    /** @brief Sets selected category. @param categoryId Category ID. */
    void setSelectedCategoryId(int categoryId);

    /** @brief Sets selected account. @param financialAccountId Account ID. */
    void setSelectedFinancialAccountId(int financialAccountId);

    /**
     * @brief Refreshes categories list.
     * @param categories New list.
     * @param selectedId ID to re-select.
     */
    void refreshCategories(const QVector<Category>& categories, int selectedId = -1);

    /**
     * @brief Refreshes accounts list.
     * @param accounts New list.
     * @param selectedId ID to re-select.
     */
    void refreshFinancialAccounts(const QVector<FinancialAccount>& accounts, int selectedId = -1);

signals:
    /**
     * @brief Request to quick-add a category.
     * @param name Name of new category.
     */
    void addCategoryRequested(const QString& name);

    /**
     * @brief Request to quick-add an account.
     * @param name Account name.
     * @param type Account type.
     * @param balance Initial balance.
     */
    void addFinancialAccountRequested(const QString& name, const QString& type, double balance);

private slots:
	void onCategoryIndexChanged(int index);
	void onFinancialAccountIndexChanged(int index);
	void onTransactionTypeChanged(int index);

private:
	Ui::AddTransactionDialogView* ui;

	int previousCategoryIndex = -1;
	int previousFinancialAccountIndex = -1;
	int previousTypeIndex = -1;

	void setupStyle();
};