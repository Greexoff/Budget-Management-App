#include <View/TransactionEditorDialogView.h>

TransactionEditorDialogView::TransactionEditorDialogView(QWidget* parent)
	: QDialog(parent), ui(new Ui::AddTransactionDialogView) {

	ui->setupUi(this);

	ui->dateEdit->setDate(QDate::currentDate());

	connect(ui->categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TransactionEditorDialogView::onCategoryIndexChanged);
	connect(ui->accountCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TransactionEditorDialogView::onFinancialAccountIndexChanged);
	connect(ui->typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TransactionEditorDialogView::onTransactionTypeChanged);
}

TransactionEditorDialogView::~TransactionEditorDialogView() {
	delete ui;
}


//----------------Setting up view-------------------------------------------------


//Method that shows available categories in combo box
void TransactionEditorDialogView::setCategories(const QVector<Category>& categories) {
	ui->categoryCombo->blockSignals(true);
	ui->categoryCombo->clear();

	for (const auto& cat : categories) {
		ui->categoryCombo->addItem(cat.getCategoryName(), cat.getCategoryId());
	}

	ui->categoryCombo->addItem(tr("-- Add new category --"), 0);

	ui->categoryCombo->setCurrentIndex(-1); 
	previousCategoryIndex = -1;
	ui->categoryCombo->blockSignals(false);
}

//Method that shows available financial accounts in combo box
void TransactionEditorDialogView::setFinancialAccounts(const QVector<FinancialAccount>& financialAccounts) {
	ui->accountCombo->blockSignals(true);
	ui->accountCombo->clear();

	for (const auto& acc : financialAccounts) {
		QString label = QString("%1 (%2)").arg(acc.getFinancialAccountName(), acc.getFinancialAccountType());
		ui->accountCombo->addItem(label, acc.getFinancialAccountId());
	}

	ui->accountCombo->addItem(tr("-- Add new account --"), 0);

	ui->accountCombo->setCurrentIndex(-1);
	previousFinancialAccountIndex = -1;
	ui->accountCombo->blockSignals(false);
}

//Method that shows available transaction types in combo box
void TransactionEditorDialogView::setTransactionTypes()
{
	ui->typeCombo->blockSignals(true);
	ui->typeCombo->clear();

	ui->typeCombo->addItem("Income", "Income");
	ui->typeCombo->addItem("Expense", "Expense");

	ui->typeCombo->setCurrentIndex(-1);
	previousTypeIndex = -1;
	ui->typeCombo->blockSignals(false);
}


void TransactionEditorDialogView::setName(const QString& name) const
{
	ui->nameEdit->setText(name);
}


//-----------Setting up data of transaction while editing transaction-------------------


void TransactionEditorDialogView::setAmount(double amount) const
{
	ui->amountSpin->setValue(amount);
}

void TransactionEditorDialogView::setDate(const QDate& date) const
{
	ui->dateEdit->setDate(date);
}

void TransactionEditorDialogView::setDescription(const QString& description) const
{
	ui->descriptionEdit->setText(description);
}

void TransactionEditorDialogView::setSelectedCategoryId(int categoryId) {
	int index = ui->categoryCombo->findData(categoryId);
	if (index != -1) {
		ui->categoryCombo->setCurrentIndex(index);
		previousCategoryIndex = index;
	}
}

void TransactionEditorDialogView::setType(const QString& typeName)
{
	int index = ui->typeCombo->findData(typeName);
	if (index != -1)
	{
		ui->typeCombo->setCurrentIndex(index);
		previousTypeIndex = index;
	}
}

void TransactionEditorDialogView::setSelectedFinancialAccountId(int financialAccountId) {
	int index = ui->accountCombo->findData(financialAccountId);
	if (index != -1) {
		ui->accountCombo->setCurrentIndex(index);
		previousFinancialAccountIndex = index;
	}
}


//----------------Getting data from user input------------------------------------------


//Method responsible for getting id of a selected category (or creating a new category)
void TransactionEditorDialogView::onCategoryIndexChanged(int index) {

	int id = ui->categoryCombo->itemData(index).toInt();

	if (id == 0) {
		bool ok;
		QString name = QInputDialog::getText(this, tr("New Category"),
			tr("Category Name:"), QLineEdit::Normal, "", &ok);

		if (ok && !name.trimmed().isEmpty()) {
			emit addCategoryRequested(name);
		}
		else {
			ui->categoryCombo->blockSignals(true);
			ui->categoryCombo->setCurrentIndex(previousCategoryIndex);
			ui->categoryCombo->blockSignals(false);
		}
	}
	else {
		previousCategoryIndex = index;
	}
}

//Method responsible for getting id of a selected financial account (or creating a new financial account)
void TransactionEditorDialogView::onFinancialAccountIndexChanged(int index)
{
	int id = ui->accountCombo->itemData(index).toInt();

	if (id == 0) {
		QDialog dlg(this);
		dlg.setWindowTitle(tr("New Financial Account"));
		QFormLayout form(&dlg);

		QLineEdit* nameEdit = new QLineEdit(&dlg);
		nameEdit->setPlaceholderText("e.g. mBank, Wallet");
		QComboBox* typeCombo = new QComboBox(&dlg);
		typeCombo->addItems({ "Cash", "Bank Account", "Savings", "Credit Card" });
		QDoubleSpinBox* balanceSpin = new QDoubleSpinBox(&dlg);
		balanceSpin->setRange(-1000000, 1000000);
		balanceSpin->setSuffix(" PLN");

		form.addRow(tr("Name:"), nameEdit);
		form.addRow(tr("Type:"), typeCombo);
		form.addRow(tr("Balance:"), balanceSpin);

		QDialogButtonBox btns(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
		form.addRow(&btns);

		connect(&btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
		connect(&btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

		if (dlg.exec() == QDialog::Accepted) {
			QString name = nameEdit->text();
			if (!name.trimmed().isEmpty()) {
				emit addFinancialAccountRequested(name, typeCombo->currentText(), balanceSpin->value());
			}
		}
		else {
			ui->accountCombo->blockSignals(true);
			ui->accountCombo->setCurrentIndex(previousFinancialAccountIndex);
			ui->accountCombo->blockSignals(false);
		}
	}
	else {
		previousFinancialAccountIndex = index;
	}
}

//Method responsible for getting id of a selected transaction type
void TransactionEditorDialogView::onTransactionTypeChanged(int index)
{
	int idx = ui->typeCombo->itemData(index).toInt();

	previousTypeIndex = idx;
}

//Method responsible for refreshing combo box of categories if change occured
void TransactionEditorDialogView::refreshCategories(const QVector<Category>& categories, int selectedId)
{
	setCategories(categories);

	if (selectedId > 0) setSelectedCategoryId(selectedId);
}

//Method responsible for refreshing combo box of financial accounts if change occured
void TransactionEditorDialogView::refreshFinancialAccounts(const QVector<FinancialAccount>& accounts, int selectedId)
{
	setFinancialAccounts(accounts);
	if (selectedId > 0) setSelectedFinancialAccountId(selectedId);
}

QString TransactionEditorDialogView::getName() const 
{
	return ui->nameEdit->text();
}

double TransactionEditorDialogView::getAmount() const 
{
	return ui->amountSpin->value();
}

QDate TransactionEditorDialogView::getDate() const 
{
	return ui->dateEdit->date();
}

QString TransactionEditorDialogView::getDescription() const 
{
	return ui->descriptionEdit->text();
}

int TransactionEditorDialogView::getSelectedCategoryId() const {
	return ui->categoryCombo->currentData().toInt();
}

QString TransactionEditorDialogView::getType() const
{
	return ui->typeCombo->currentData().toString();
}

int TransactionEditorDialogView::getSelectedFinancialAccountId() const {
	return ui->accountCombo->currentData().toInt();
}
