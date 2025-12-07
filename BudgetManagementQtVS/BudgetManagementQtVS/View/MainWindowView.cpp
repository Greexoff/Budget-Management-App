#include "View/MainWindowView.h"
#include "ui_MainWindowView.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QHeaderView>  

/**
 * @brief Constructs the main window and initializes UI components
 * @param parent Parent widget
 */
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), TableModel(new QStandardItemModel(this))
{
	ui->setupUi(this);

    initializeTransactionTable();
	setupConnections();
}

/**
 * @brief Destructor - cleans up UI resources
 */
MainWindow::~MainWindow()
{
	delete ui;
}

/**
 * @brief Initializes the transaction table with columns and display settings
 */
void MainWindow::initializeTransactionTable()
{
    // Define table columns
    TableModel->setColumnCount(7);
    TableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    TableModel->setHeaderData(1, Qt::Horizontal, tr("Nazwa"));
    TableModel->setHeaderData(2, Qt::Horizontal, tr("Data"));
    TableModel->setHeaderData(3, Qt::Horizontal, tr("Opis"));
    TableModel->setHeaderData(4, Qt::Horizontal, tr("Kwota"));
    TableModel->setHeaderData(5, Qt::Horizontal, tr("Typ"));
    TableModel->setHeaderData(6, Qt::Horizontal, tr("Kategoria"));

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
void MainWindow::setupConnections() 
{
	connect(ui->buttonAddTransaction, &QPushButton::clicked, this, &MainWindow::onButtonAddTransactionClicked);
	connect(ui->buttonDeleteTransaction, &QPushButton::clicked, this, &MainWindow::onButtonDeleteTransactionClicked);
    connect(ui->browseCategoriesButton, &QPushButton::clicked, this, &MainWindow::onButtonManageCategoriesClicked);
}

/**
 * @brief Handles click on Add Transaction button
 */
void MainWindow::onButtonAddTransactionClicked()
{
	emit addTransactionRequested();
}

/**
 * @brief Handles click on Delete Transaction button
 */
void MainWindow::onButtonDeleteTransactionClicked()
{
	emit deleteTransactionRequested();
}

/**
 * @brief Handles click on Manage Categories button
 */
void MainWindow::onButtonManageCategoriesClicked()
{
    emit manageCategoriesRequested();
}

/**
 * @brief Updates the transaction table with new data
 * @param rows Vector of transaction data rows
 */
void MainWindow::setTransactionRows(const QVector<QStringList>& rows)
{
    TableModel->clear();

    // Set column headers
    TableModel->setHorizontalHeaderLabels({ "ID", "Nazwa", "Data", "Opis", "Kwota", "Typ", "Kategoria" });

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
int MainWindow::selectedTranstacionId() const
{
    QModelIndex index = ui->TransactionTabelView->currentIndex();
    if (!index.isValid())
        return -1;

    bool ok = false;
    int id = TableModel->data(TableModel->index(index.row(), 0)).toInt(&ok);
    return ok ? id : -1;
}
