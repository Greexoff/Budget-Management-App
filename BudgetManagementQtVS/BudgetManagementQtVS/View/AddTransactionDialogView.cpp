#include <View/AddTransactionDialogView.h>

AddTransactionDialogView::AddTransactionDialogView(QWidget* parent)
	: QDialog(parent), ui(new Ui::AddTransactionDialogView) {

	ui->setupUi(this);

	ui->dateEdit->setDate(QDate::currentDate());
}

AddTransactionDialogView::~AddTransactionDialogView() {
	delete ui;
}

void AddTransactionDialogView::setCategories(const QVector<Category>& categories) {
	ui->categoryCombo->clear();

	for (const auto& cat : categories) {
		ui->categoryCombo->addItem(cat.getCategoryName(), cat.getCategoryId());
	}
}

void AddTransactionDialogView::setFinancialAccounts(const QVector<FinancialAccount>& financialAccounts) {
	ui->accountCombo->clear();

	for (const auto& acc : financialAccounts) {
		QString label = QString("%1 (%2)").arg(acc.getFinancialAccountName(), acc.getFinancialAccountType());
		ui->accountCombo->addItem(label, acc.getFinancialAccountId());
	}
}
void AddTransactionDialogView::setTransactionType(const QVector<TransactionType>& transactionType)
{
	ui->typeCombo->clear();
	for (const auto& type : transactionType) {
		//ui->typeCombo->addItem(getTransactionTypeName(type));
	}
}

QString AddTransactionDialogView::getName() const { return ui->nameEdit->text(); }
double AddTransactionDialogView::getAmount() const { return ui->amountSpin->value(); }
QDate AddTransactionDialogView::getDate() const { return ui->dateEdit->date(); }
QString AddTransactionDialogView::getDescription() const { return ui->descriptionEdit->text(); }

int AddTransactionDialogView::getSelectedCategoryId() const {
	return ui->categoryCombo->currentData().toInt();
}

int AddTransactionDialogView::getSelectedFinancialAccountId() const {
	return ui->accountCombo->currentData().toInt();
}

void AddTransactionDialogView::setName(const QString& name) {
	ui->nameEdit->setText(name);
}

void AddTransactionDialogView::setAmount(double amount) {
	ui->amountSpin->setValue(amount);
}

void AddTransactionDialogView::setDate(const QDate& date) {
	ui->dateEdit->setDate(date);
}

void AddTransactionDialogView::setDescription(const QString& description) {
	ui->descriptionEdit->setText(description);
}

void AddTransactionDialogView::setSelectedCategoryId(int categoryId) {
	int index = ui->categoryCombo->findData(categoryId);
	if (index != -1) {
		ui->categoryCombo->setCurrentIndex(index);
	}
}

void AddTransactionDialogView::setSelectedFinancialAccountId(int financialAccountId) {
	int index = ui->accountCombo->findData(financialAccountId);
	if (index != -1) {
		ui->accountCombo->setCurrentIndex(index);
	}
}