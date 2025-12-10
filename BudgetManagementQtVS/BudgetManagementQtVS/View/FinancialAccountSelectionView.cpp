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
	bool ok = false;
	QString name = QInputDialog::getText(this, tr("New financial account"), tr("Financial account name:"), QLineEdit::Normal, "", &ok);
	if (!ok || name.trimmed().isEmpty())
	{
		return;
	}
	QString type = QInputDialog::getText(this, tr("New financial account"), tr("Financial account type:"), QLineEdit::Normal, "", &ok);
	if (!ok || type.trimmed().isEmpty())
	{
		return;
	}
	double balance = QInputDialog::getDouble(this, tr("New financial account"), tr("Financial account balance:"), 0.0, -1e9, 1e9, 2, &ok);
	if (!ok )
	{
		return;
	}
	emit addRequestedFinancialAccount(name,type,balance);
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

	// Sprawdzenie, czy coś jest zaznaczone
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

	bool ok = false;
	QString newName = QInputDialog::getText(this, tr("Edit Account"), tr("Name:"), QLineEdit::Normal, currentFinancialAccount.getFinancialAccountName(), &ok);
	if (!ok || newName.trimmed().isEmpty()) return;

	QString newType = QInputDialog::getText(this, tr("Edit Account"), tr("Type:"), QLineEdit::Normal, currentFinancialAccount.getFinancialAccountType(), &ok);
	if (!ok || newType.trimmed().isEmpty()) return;

	double newBalance = QInputDialog::getDouble(this, tr("Edit Account"), tr("Balance:"), currentFinancialAccount.getFinancialAccountBalance(), -1e9, 1e9, 2, &ok);
	if (!ok) return;

	emit editRequestedFinancialAccount(currentFinancialAccount.getFinancialAccountId(), newName, newType, newBalance);
}