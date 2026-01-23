/**
 * @file TransactionWindowView.cpp
 * @brief Implementation of the Transaction Window View.
 */
#include "View/TransactionWindowView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

 /** @brief Constructor. Initializes UI, Style, and Connections. */
TransactionWindow::TransactionWindow(QWidget* parent)
    : QWidget(parent), tableModel(new QStandardItemModel(this))
{
    setupUI();
    setupStyle();
    setupConnections();
}
/** @brief Helper to create standard buttons with connections. */
QPushButton* TransactionWindow::createButton(const QString& text, const QString& objName, void (TransactionWindow::* slot)())
{
    QPushButton* btn = new QPushButton(text);
    btn->setObjectName(objName);
    connect(btn, &QPushButton::clicked, this, slot);
    return btn;
}
/** @brief Creates the Header section (Label + Search). */
QLayout* TransactionWindow::createHeaderSection()
{
    QHBoxLayout* layout = new QHBoxLayout();

    QLabel* viewLabel = new QLabel("Transactions Overview");
    viewLabel->setObjectName("viewLabel");

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search transactions...");
    searchEdit->setFixedWidth(300);

    layout->addWidget(viewLabel);
    layout->addStretch();
    layout->addWidget(searchEdit);

    return layout;
}
/** @brief Creates the Budget section (Progress Bar + Set Limit button). */
QWidget* TransactionWindow::createBudgetSection()
{
    QFrame* frame = new QFrame();
    frame->setObjectName("budgetFrame");
    QVBoxLayout* layout = new QVBoxLayout(frame);

    QHBoxLayout* header = new QHBoxLayout();
    budgetLabel = new QLabel("Monthly Budget Usage");

    actionButtons["budget"] = createButton("Set Limit", "actionButton", &TransactionWindow::onButtonBudgetClicked);
    actionButtons["budget"]->setFixedWidth(100);

    header->addWidget(budgetLabel);
    header->addStretch();
    header->addWidget(actionButtons["budget"]);

    budgetProgressBar = new QProgressBar();
    budgetProgressBar->setFixedHeight(15);
    budgetProgressBar->setTextVisible(false);

    layout->addLayout(header);
    layout->addWidget(budgetProgressBar);

    return frame;
}
/** @brief Creates the Action section (Add, Edit, Delete buttons). */
QLayout* TransactionWindow::createActionSection()
{
    QHBoxLayout* layout = new QHBoxLayout();

    actionButtons["add"] = createButton("+ Add Transaction", "actionButtonAdd", &TransactionWindow::onButtonAddClicked);
    actionButtons["edit"] = createButton("Edit", "actionButton", &TransactionWindow::onButtonEditClicked);
    actionButtons["delete"] = createButton("Delete", "actionButtonDelete", &TransactionWindow::onButtonDeleteClicked);

    layout->addWidget(actionButtons["add"]);
    layout->addWidget(actionButtons["edit"]);
    layout->addWidget(actionButtons["delete"]);
    layout->addStretch();

    return layout;
}
/** @brief Assembles the main layout components. */
void TransactionWindow::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    mainLayout->addLayout(createHeaderSection());
    mainLayout->addWidget(createBudgetSection());
    mainLayout->addLayout(createActionSection());

    transactionTable = new QTableView();
    initializeTable();
    mainLayout->addWidget(transactionTable);
}
/** @brief Configures table model headers and view properties. */
void TransactionWindow::initializeTable() const
{
    transactionTable->setModel(tableModel);
    QStringList headers = { "ID", "Name", "Date", "Description", "Amount", "Type", "Category", "Account" };
    tableModel->setHorizontalHeaderLabels(headers);

    transactionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    transactionTable->setColumnHidden(0, true);
    transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    transactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionTable->verticalHeader()->setVisible(false);
    transactionTable->setAlternatingRowColors(true);
}
/** @brief Connects search edit and table headers to signals. */
void TransactionWindow::setupConnections()
{
    connect(searchEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        emit searchTextRequest(text);
        });

    connect(transactionTable->horizontalHeader(), &QHeaderView::sectionClicked, this, [this](int logicalIndex) {
        emit columnSortRequest(logicalIndex);
        });
}
/** @brief Clears and repopulates the table model with rows. */
void TransactionWindow::setTransactionTabHeaders(const QVector<QStringList>& rows) const
{
    tableModel->removeRows(0, tableModel->rowCount());
    for (const QStringList& row : rows) {
        QList<QStandardItem*> items;
        for (const QString& value : row) items.append(new QStandardItem(value));
        tableModel->appendRow(items);
    }
}
/** @brief Calculates percentage and updates progress bar and label text. */
void TransactionWindow::updateBudgetDisplay(double limit, double spent) const
{
    double percentage = (limit > 0) ? (spent / limit) * 100.0 : 0.0;
    budgetProgressBar->setValue(percentage > 100 ? 100 : static_cast<int>(percentage));
    budgetLabel->setText(QString("Budget: %1 / %2 PLN (Remaining: %3 PLN)")
        .arg(spent, 0, 'f', 2).arg(limit, 0, 'f', 2).arg(limit - spent, 0, 'f', 2));
}
/** @brief Returns ID from the first column of the selected row. */
int TransactionWindow::getSelectedTransactionId() const
{
    QModelIndex index = transactionTable->currentIndex();
    if (!index.isValid()) return -1;
    return tableModel->data(tableModel->index(index.row(), 0)).toInt();
}
/** @brief Display helper for QMessageBox. */
void TransactionWindow::showTransactionMessage(const QString& header, const QString& message, const QString& messageType)
{
    if (messageType == "error") QMessageBox::warning(this, header, message);
    else QMessageBox::information(this, header, message);
}
/** @brief Clears search text. */
void TransactionWindow::clearSearchEdit() const { searchEdit->clear(); }
/** @brief Sets CSS styling. */
void TransactionWindow::setupStyle()
{
    this->setStyleSheet(
        "QWidget { color: white; background-color: transparent; }"
        "QLabel#viewLabel { font-size: 24px; font-weight: bold; color: white; }"
        "QLineEdit { padding: 8px; border: 1px solid #444444; border-radius: 5px; background-color: #2d2d2d; color: white; }"
        "QFrame#budgetFrame { background-color: #1e1e1e; border-radius: 10px; border: 1px solid #333333; padding: 10px; }"
        "QProgressBar { border: none; background-color: #2d2d2d; border-radius: 7px; text-align: center; }"
        "QProgressBar::chunk { background-color: #2ecc71; border-radius: 7px; }"
        "QPushButton#actionButtonAdd { background-color: #27ae60; color: white; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton#actionButtonDelete { background-color: #c0392b; color: white; border-radius: 5px; padding: 8px 15px; }"
        "QPushButton#actionButton { background-color: #2980b9; color: white; border-radius: 5px; padding: 8px 15px; }"
        "QTableView { border: 1px solid #333333; background-color: #1e1e1e; color: white; selection-background-color: #3498db; }"
        "QHeaderView::section { background-color: #2d2d2d; padding: 8px; border: 1px solid #333333; font-weight: bold; }"
    );
}