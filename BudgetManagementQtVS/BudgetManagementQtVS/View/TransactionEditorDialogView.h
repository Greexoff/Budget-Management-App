#pragma once

#include <QDialog>
#include <QInputDialog>
#include <ui_TransactionEditorDialogView.h>
#include <Model/Category.h>
#include <Model/FinancialAccount.h>
#include <Model/Transaction.h>

class TransactionEditorDialogView : public QDialog {
	Q_OBJECT

public:
	explicit TransactionEditorDialogView(QWidget* parent = nullptr);
	~TransactionEditorDialogView();

	void setCategories(const QVector<Category>& categories);
	void setFinancialAccounts(const QVector<FinancialAccount>& financialAccounts);
	void setTransactionTypes();

	QString getName() const;
	double getAmount() const;
	QDate getDate() const;
	QString getDescription() const;
	QString getType() const;
	int getSelectedCategoryId() const;
	int getSelectedFinancialAccountId() const;

	void setName(const QString& name);
	void setAmount(double amount);
	void setDate(const QDate& date);
	void setDescription(const QString& description);
	void setType(const QString& type);
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
	void onTransactionTypeChanged(int index);

private:
	Ui::AddTransactionDialogView* ui;

	int previousCategoryIndex = -1;
	int previousFinancialAccountIndex = -1;
	int previousTypeIndex = -1;
};