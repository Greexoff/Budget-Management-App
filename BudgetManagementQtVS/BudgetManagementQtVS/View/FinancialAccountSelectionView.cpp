#include "FinancialAccountSelectionView.h"

FinancialAccountSelectionView::FinancialAccountSelectionView(QWidget* parent)
	: QDialog(parent), ui(new Ui::FinancialAccountSelectionView) {

	ui->setupUi(this);
	setWindowTitle("Browse financial accounts");
	setupTable();
	connectMethodToButton();
}

FinancialAccountSelectionView::~FinancialAccountSelectionView() {
	delete ui;
}

void FinancialAccountSelectionView::connectMethodToButton() {
	connect(ui->selectFinancialAccountButton, &QPushButton::clicked, this, &FinancialAccountSelectionView::selectFinancialAccountButtonClicked);
	connect(ui->addFinancialAccountButton, &QPushButton::clicked, this, &FinancialAccountSelectionView::addFinancialAccountButtonClicked);
	connect(ui->deleteFinancialAccountButton, &QPushButton::clicked, this, &FinancialAccountSelectionView::deleteFinancialAccountButtonClicked);
	connect(ui->cancelFinancialAccountButton, &QPushButton::clicked, this, &FinancialAccountSelectionView::cancelFinancialAccountButtonClicked);
	connect(ui->editFinancialAccountButton, &QPushButton::clicked, this, &FinancialAccountSelectionView::editFinancialAccountButtonClicked);
	connect(ui->searchEdit, &QLineEdit::textChanged, this, &FinancialAccountSelectionView::searchTextChanged);
}

void FinancialAccountSelectionView::setupTable() {
	ui->financialAccountsTable->setColumnCount(3);

	ui->financialAccountsTable->setHorizontalHeaderLabels({ "Name", "Type", "Balance" });

	ui->financialAccountsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	ui->financialAccountsTable->verticalHeader()->setVisible(false);
	ui->financialAccountsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->financialAccountsTable->setSelectionMode(QAbstractItemView::SingleSelection);
}


void FinancialAccountSelectionView::setFinancialAccounts(const QVector<FinancialAccount>& financialAccounts) {
	financialAccountId = financialAccounts;

	ui->financialAccountsTable->setRowCount(0);

	for (const auto& account : financialAccountId) {

		int row = ui->financialAccountsTable->rowCount();
		ui->financialAccountsTable->insertRow(row);

		ui->financialAccountsTable->setItem(row, 0, new QTableWidgetItem(account.getFinancialAccountName()));

		ui->financialAccountsTable->setItem(row, 1, new QTableWidgetItem(account.getFinancialAccountType()));

		QString balanceStr = QString::number(account.getFinancialAccountBalance(), 'f', 2) + " PLN";
		ui->financialAccountsTable->setItem(row, 2, new QTableWidgetItem(balanceStr));
	}
}

void FinancialAccountSelectionView::selectFinancialAccountButtonClicked() {

	int row = ui->financialAccountsTable->currentRow();
	if (row < 0 || row >= financialAccountId.size()) {
		return;
	}

	selectedFinancialAccountId = financialAccountId[row].getFinancialAccountId();
	emit selectRequestedFinancialAccount(financialAccountId[row].getFinancialAccountId());
	accept();
}

void FinancialAccountSelectionView::addFinancialAccountButtonClicked() {
	QDialog dlg(this);
	dlg.setWindowTitle(tr("New Financial Account"));

	QFormLayout* layout = new QFormLayout(&dlg);

	QLineEdit* nameEdit = new QLineEdit(&dlg);
	nameEdit->setPlaceholderText("e.g. mBank, Wallet");

	QComboBox* typeCombo = new QComboBox(&dlg);
	typeCombo->addItems({ "Cash", "Bank Account", "Savings", "Credit Card" });

	QDoubleSpinBox* balanceSpin = new QDoubleSpinBox(&dlg);
	balanceSpin->setRange(-10000000.0, 10000000.0); 
	balanceSpin->setDecimals(2);
	balanceSpin->setSuffix(" PLN");

	layout->addRow(tr("Account Name:"), nameEdit);
	layout->addRow(tr("Account Type:"), typeCombo);
	layout->addRow(tr("Initial Balance:"), balanceSpin);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dlg);
	layout->addRow(buttonBox);

	connect(buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Accepted) {
		QString name = nameEdit->text();

		if (name.trimmed().isEmpty()) {
			return;
		}

		emit addRequestedFinancialAccount(name, typeCombo->currentText(), balanceSpin->value());
	}
}

void FinancialAccountSelectionView::deleteFinancialAccountButtonClicked() {
	
	int row = ui->financialAccountsTable->currentRow();
	if (row < 0 || row >= financialAccountId.size()) {
		return;
	}

	emit deleteRequestedFinancialAccount(financialAccountId[row].getFinancialAccountId());
}

void FinancialAccountSelectionView::cancelFinancialAccountButtonClicked() {
	reject();
}

void FinancialAccountSelectionView::setSelectFinancialAccountButtonVisible(bool visible) {
	if (ui->selectFinancialAccountButton) {
		ui->selectFinancialAccountButton->setVisible(visible);
	}
}

void FinancialAccountSelectionView::showFinancialAccountMessage(QString header, QString message, QString messageType) {
	if (messageType == "error")
	{
		QMessageBox::warning(this, header, message);
	}
	else
	{
		QMessageBox::information(this, header, message);
	}
}

void FinancialAccountSelectionView::editFinancialAccountButtonClicked() {
	int row = ui->financialAccountsTable->currentRow();

	if (row < 0 || row >= financialAccountId.size()) {
		return;
	}
	FinancialAccount currentFinancialAccount = financialAccountId[0]; 

	QString currentNameInTable = ui->financialAccountsTable->item(row, 0)->text();
	for (const auto& acc : financialAccountId) {
		if (acc.getFinancialAccountName() == currentNameInTable) {
			currentFinancialAccount = acc;
			break;
		}
	}

	if (currentFinancialAccount.getFinancialAccountId() == 1) {
		showFinancialAccountMessage(tr("Error"), tr("Cannot edit default account."), "error");
		return;
	}

	QDialog dlg(this);
	dlg.setWindowTitle(tr("Edit Financial Account"));
	QFormLayout* layout = new QFormLayout(&dlg);

	QLineEdit* nameEdit = new QLineEdit(&dlg);
	nameEdit->setText(currentFinancialAccount.getFinancialAccountName()); 

	QComboBox* typeCombo = new QComboBox(&dlg);
	typeCombo->addItems({ "Cash", "Bank Account", "Savings", "Credit Card" });
	typeCombo->setCurrentText(currentFinancialAccount.getFinancialAccountType()); 

	QDoubleSpinBox* balanceSpin = new QDoubleSpinBox(&dlg);
	balanceSpin->setRange(-10000000.0, 10000000.0);
	balanceSpin->setSuffix(" PLN");
	balanceSpin->setValue(currentFinancialAccount.getFinancialAccountBalance()); 

	layout->addRow(tr("Name:"), nameEdit);
	layout->addRow(tr("Type:"), typeCombo);
	layout->addRow(tr("Balance:"), balanceSpin);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dlg);
	layout->addRow(buttonBox);

	connect(buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

	if (dlg.exec() == QDialog::Accepted) {
		QString newName = nameEdit->text();
		if (newName.trimmed().isEmpty()) return;

		emit editRequestedFinancialAccount(currentFinancialAccount.getFinancialAccountId(), newName, typeCombo->currentText(), balanceSpin->value());
	}
}
void FinancialAccountSelectionView::searchTextChanged(const QString& searchText)
{
	emit searchTextRequest(searchText);
}

void FinancialAccountSelectionView::clearSearchLineEdit()
{
	ui->searchEdit->clear();
}