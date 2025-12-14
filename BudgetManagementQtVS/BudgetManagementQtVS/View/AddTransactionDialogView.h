#pragma once

#include <QDialog>
#include <QInputDialog>
#include <ui_AddTransactionDialogView.h>
#include <Model/Category.h>
#include <Model/FinancialAccount.h>
#include <Model/Transaction.h>

class AddTransactionDialogView : public QDialog {
	Q_OBJECT

public:
	explicit AddTransactionDialogView(QWidget* parent = nullptr);
	~AddTransactionDialogView();

	void setCategories(const QVector<Category>& categories);
	void setFinancialAccounts(const QVector<FinancialAccount>& financialAccounts);
	void setTransactionType(const QVector<TransactionType>& transactionType);

	QString getName() const;
	double getAmount() const;
	QDate getDate() const;
	QString getDescription() const;
	int getSelectedCategoryId() const;
	int getSelectedFinancialAccountId() const;

	void setName(const QString& name);
	void setAmount(double amount);
	void setDate(const QDate& date);
	void setDescription(const QString& description);
	void setSelectedCategoryId(int categoryId);
	void setSelectedFinancialAccountId(int financialAccountId);

	void refreshCategories(const QVector<Category>& categories, int selectedId = -1);
	void refreshFinancialAccounts(const QVector<FinancialAccount>& accounts, int selectedId = -1);

signals:
	void addCategoryRequested(const QString& name);
	void addFinancialAccountRequested(const QString& name, const QString& type, double balance);

private slots:
	void onCategoryIndexChanged(int index);
	void onFinancialAccountIndexChanged(int index);

private:
	Ui::AddTransactionDialogView* ui;

	int previousCategoryIndex = -1;
	int previousFinancialAccountIndex = -1;
};