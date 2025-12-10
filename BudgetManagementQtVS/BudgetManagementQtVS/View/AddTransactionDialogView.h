#pragma once

#include <QDialog>
#include <ui_AddTransactionDialogView.h>
#include <Model/Category.h>
#include <Model/FinancialAccount.h>

class AddTransactionDialogView : public QDialog {
	Q_OBJECT

public:
	explicit AddTransactionDialogView(QWidget* parent = nullptr);
	~AddTransactionDialogView();

	void setCategories(const QVector<Category>& categories);
	void setFinancialAccounts(const QVector<FinancialAccount>& financialAccounts);

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

private:
	Ui::AddTransactionDialogView* ui;
};