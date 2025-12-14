#pragma once

#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>
#include "ui_FinancialAccountSelectionView.h"
#include "Model/FinancialAccount.h"

class FinancialAccountSelectionView : public QDialog {
	Q_OBJECT

public:
	explicit FinancialAccountSelectionView(QWidget* parent = nullptr);
	~FinancialAccountSelectionView();

	void setFinancialAccounts(const QVector<FinancialAccount>& financialAccounts);

	void setSelectFinancialAccountButtonVisible(bool visible);

	int getSelectedFinancialAccountId() const { return selectedFinancialAccountId; }

	void clearSearchLineEdit();

public slots:
	void showFinancialAccountMessage(QString header, QString message, QString messageType);

signals:

	void selectRequestedFinancialAccount(int financialAccountId);

	void addRequestedFinancialAccount(const QString& financialAccountName, const QString& financialAccountType, double financialAccount_balance);

	void deleteRequestedFinancialAccount(int financialAccountId);

	void editRequestedFinancialAccount(int financialAccountId, const QString& name, const QString& type, double balance);

	void searchTextRequest(const QString& searchText);

private slots:
	void selectFinancialAccountButtonClicked();
	void addFinancialAccountButtonClicked();
	void deleteFinancialAccountButtonClicked();
	void editFinancialAccountButtonClicked();
	void cancelFinancialAccountButtonClicked();
	void searchTextChanged(const QString& searchText);

private:
	Ui::FinancialAccountSelectionView* ui;
	QVector<FinancialAccount> financialAccountId;
	int selectedFinancialAccountId = -1;

	void connectMethodToButton();

	void setupTable();
};