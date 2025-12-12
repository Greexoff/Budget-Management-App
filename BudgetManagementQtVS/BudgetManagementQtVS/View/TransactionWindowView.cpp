#include "View/TransactionWindowView.h"
#include "ui_TransactionWindowView.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QHeaderView>  

/**
 * @brief Constructs the main window and initializes UI components
 * @param parent Parent widget
 */
TransactionWindow::TransactionWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), TableModel(new QStandardItemModel(this))
{
	ui->setupUi(this);

    initializeTransactionTable();
    setWindowTitle("Browse transactions");
	setupConnections();
}

/**
 * @brief Destructor - cleans up UI resources
 */
TransactionWindow::~TransactionWindow()
{
	delete ui;
}

/**
 * @brief Initializes the transaction table with columns and display settings
 */
void TransactionWindow::initializeTransactionTable()
{
    // Define table columns
    TableModel->setColumnCount(8);
    TableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    TableModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    TableModel->setHeaderData(2, Qt::Horizontal, tr("Date"));
    TableModel->setHeaderData(3, Qt::Horizontal, tr("Description"));
    TableModel->setHeaderData(4, Qt::Horizontal, tr("Amount"));
    TableModel->setHeaderData(5, Qt::Horizontal, tr("Type"));
    TableModel->setHeaderData(6, Qt::Horizontal, tr("Category"));
    TableModel->setHeaderData(7, Qt::Horizontal, tr("Financial Account"));

    // Apply model to table view
    ui->TransactionTabelView->setModel(TableModel);

    // Configure selection behavior
    ui->TransactionTabelView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TransactionTabelView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->TransactionTabelView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Configure column sizing
    ui->TransactionTabelView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Hide ID column 
    ui->TransactionTabelView->setColumnHidden(0, true);
}

/**
 * @brief Connects UI buttons to their handler methods
 */
void TransactionWindow::setupConnections() 
{
	connect(ui->buttonAddTransaction, &QPushButton::clicked, this, &TransactionWindow::onButtonAddTransactionClicked);
	connect(ui->buttonDeleteTransaction, &QPushButton::clicked, this, &TransactionWindow::onButtonDeleteTransactionClicked);
    connect(ui->browseCategoriesButton, &QPushButton::clicked, this, &TransactionWindow::onButtonManageCategoriesClicked);
    connect(ui->buttonEdit, &QPushButton::clicked, this, &TransactionWindow::onButtonEditTransactionClicked);
    connect(ui->changeProfileButton, &QPushButton::clicked, this, &TransactionWindow::onButtonChangeProfileClicked);
    connect(ui->browseFinancialAccountsButton, &QPushButton::clicked, this, &TransactionWindow::onButtonManageFinancialAccountsClicked);
    connect(ui->editBudgetButton, &QPushButton::clicked, this, &TransactionWindow::onButtonEditBudgetClicked);

    QHeaderView* header = ui->TransactionTabelView->horizontalHeader();
    connect(header, &QHeaderView::sectionClicked, this, &TransactionWindow::onColumnHeaderClicked);
}

/**
 * @brief Handles click on Add Transaction button
 */
void TransactionWindow::onButtonAddTransactionClicked()
{
	emit addTransactionRequest();
}

/**
 * @brief Handles click on Delete Transaction button
 */
void TransactionWindow::onButtonDeleteTransactionClicked()
{
	emit deleteTransactionRequest();
}

/**
 * @brief Handles click on Manage Categories button
 */
void TransactionWindow::onButtonManageCategoriesClicked()
{
    emit showCategoriesRequest();
}

void TransactionWindow::onButtonManageFinancialAccountsClicked()
{
    emit showFinancialAccountsRequest();
}

/**
 * @brief Updates the transaction table with new data
 * @param rows Vector of transaction data rows
 */
void TransactionWindow::setTransactionRows(const QVector<QStringList>& rows)
{
    TableModel->clear();

    // Set column headers
    TableModel->setHorizontalHeaderLabels({ "ID", "Name", "Date", "Description", "Amount", "Type", "Category","Financial account"});

    // Add transaction rows
    for (const QStringList& row : rows) {
        QList<QStandardItem*> items;
        for (const QString& value : row)
            items.append(new QStandardItem(value));
        TableModel->appendRow(items);
    }

    // Configure column display
    auto header = ui->TransactionTabelView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    ui->TransactionTabelView->setColumnHidden(0, true);
}

/**
 * @brief Collect the ID of the currently selected transaction
 * @return int Transaction ID, or -1 if no valid selection
 */
int TransactionWindow::getSelectedTransactionId() const
{
    QModelIndex index = ui->TransactionTabelView->currentIndex();
    if (!index.isValid())
        return -1;

    bool ok = false;
    int id = TableModel->data(TableModel->index(index.row(), 0)).toInt(&ok);
    return ok ? id : -1;
}
void TransactionWindow::showTransactionMessage(QString header, QString message, QString messageType)
{
    if (messageType == "error")
    {
        QMessageBox::warning(this, header, message);
    }
    else
    {
        QMessageBox::information(this, header, message);
    }
}

void TransactionWindow::onButtonEditTransactionClicked()
{
    emit editTransactionRequest();
}

QString TransactionWindow::getTransactionNameFromInput(bool& correctData, const QString& defaultValue)
{
    return QInputDialog::getText(this, tr("Transaction"), tr("Name:"),
        QLineEdit::Normal, defaultValue, &correctData);
}

double TransactionWindow::getTransactionAmountFromInput(bool& correctData, double defaultValue)
{
    return QInputDialog::getDouble(this, tr("Transaction"), tr("Amount:"),
        defaultValue, -1e9, 1e9, 2, &correctData);
}

QString TransactionWindow::getTransactionDescriptionFromInput(bool& correctData, const QString& defaultValue)
{
    return QInputDialog::getText(this, tr("Transaction"), tr("Description:"),
        QLineEdit::Normal, defaultValue, &correctData);
}

void TransactionWindow::onButtonChangeProfileClicked()
{
    emit backToProfileRequested();
}

void TransactionWindow::onButtonEditBudgetClicked()
{
    emit editBudgetRequest();
}

void TransactionWindow::updateBudgetDisplay(double limit, double spent)
{
    ui->budgetProgressBar->setRange(0, 100);

    double percentage = (limit > 0) ? (spent / limit) * 100.0 : 0.0;

    int visualValue = (percentage > 100.0) ? 100 : static_cast<int>(percentage);
    ui->budgetProgressBar->setValue(visualValue);

    if (spent > limit && limit > 0) {
        ui->budgetProgressBar->setStyleSheet("QProgressBar::chunk { background-color: #ff4d4d; }");
    }
    else {
        ui->budgetProgressBar->setStyleSheet("QProgressBar::chunk { background-color: #4CAF50; }"); 
    }

    double remaining = limit - spent;
    ui->budgetLabel->setText(QString("Spent: %1 / Limit: %2 PLN (Remaining: %3 PLN)")
        .arg(QString::number(spent, 'f', 2))
        .arg(QString::number(limit, 'f', 2))
        .arg(QString::number(remaining, 'f', 2)));
}
void TransactionWindow::onColumnHeaderClicked(int columnId)
{
    emit columnSortRequest(columnId);
}