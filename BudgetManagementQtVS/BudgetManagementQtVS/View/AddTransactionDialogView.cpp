#include <View/AddTransactionDialogView.h>

AddTransactionDialogView::AddTransactionDialogView(QWidget* parent)
	: QDialog(parent), ui(new Ui::AddTransactionDialogView) {

	ui->setupUi(this);

	ui->dateEdit->setDate(QDate::currentDate());

	connect(ui->categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AddTransactionDialogView::onCategoryIndexChanged);
	connect(ui->accountCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AddTransactionDialogView::onFinancialAccountIndexChanged);
	connect(ui->typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AddTransactionDialogView::onTransactionTypeChanged);
}

AddTransactionDialogView::~AddTransactionDialogView() {
	delete ui;
}

void AddTransactionDialogView::setCategories(const QVector<Category>& categories) {
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

void AddTransactionDialogView::onCategoryIndexChanged(int index) {

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

void AddTransactionDialogView::onTransactionTypeChanged(int index)
{
	int idx = ui->typeCombo->itemData(index).toInt();

	previousTypeIndex = idx;
}
void AddTransactionDialogView::refreshCategories(const QVector<Category>& categories, int selectedId)
{
	setCategories(categories);

	if (selectedId > 0) setSelectedCategoryId(selectedId);
}

void AddTransactionDialogView::setFinancialAccounts(const QVector<FinancialAccount>& financialAccounts) {
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
void AddTransactionDialogView::setTransactionTypes()
{
	ui->typeCombo->blockSignals(true);
	ui->typeCombo->clear();

	ui->typeCombo->addItem("Income", "Income");
	ui->typeCombo->addItem("Expense", "Expense");

	ui->typeCombo->setCurrentIndex(-1);
	previousTypeIndex = -1;
	ui->typeCombo->blockSignals(false);
}

void AddTransactionDialogView::onFinancialAccountIndexChanged(int index)
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

void AddTransactionDialogView::refreshFinancialAccounts(const QVector<FinancialAccount>& accounts, int selectedId)
{
	setFinancialAccounts(accounts);
	if (selectedId > 0) setSelectedFinancialAccountId(selectedId);
}

QString AddTransactionDialogView::getName() const { return ui->nameEdit->text(); }
double AddTransactionDialogView::getAmount() const { return ui->amountSpin->value(); }
QDate AddTransactionDialogView::getDate() const { return ui->dateEdit->date(); }
QString AddTransactionDialogView::getDescription() const { return ui->descriptionEdit->text(); }

int AddTransactionDialogView::getSelectedCategoryId() const {
	return ui->categoryCombo->currentData().toInt();
}

QString AddTransactionDialogView::getType() const
{
	return ui->typeCombo->currentData().toString();
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
		previousCategoryIndex = index;
	}
}

void AddTransactionDialogView::setType(const QString& typeName)
{
	int index = ui->typeCombo->findData(typeName);
	if (index != -1)
	{
		ui->typeCombo->setCurrentIndex(index);
		previousTypeIndex = index;
	}
}

void AddTransactionDialogView::setSelectedFinancialAccountId(int financialAccountId) {
	int index = ui->accountCombo->findData(financialAccountId);
	if (index != -1) {
		ui->accountCombo->setCurrentIndex(index);
		previousFinancialAccountIndex = index;
	}
}