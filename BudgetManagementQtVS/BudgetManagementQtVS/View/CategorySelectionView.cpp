#include "View/CategorySelectionView.h"
#include <QMessageBox>



CategorySelectionView::CategorySelectionView(QWidget* parent)
    : QWidget(parent), tableModel(new QStandardItemModel(this))
{
    setupStyle();
    setupUI();
}

void CategorySelectionView::setupUI()
{
    QVBoxLayout* contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(20);

    // 1. Nagłówek i Szukanie
    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* viewLabel = new QLabel("Categories Management");
    viewLabel->setObjectName("viewLabel"); 

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search categories...");
    searchEdit->setFixedWidth(300);

    headerLayout->addWidget(viewLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(searchEdit);

    // 2. Przyciski Akcji
    QHBoxLayout* actionLayout = new QHBoxLayout();

    btnAdd = new QPushButton("+ Add Category");
    btnAdd->setObjectName("actionButtonAdd");

    btnEdit = new QPushButton("Edit");
    btnEdit->setObjectName("actionButton");

    btnDelete = new QPushButton("Delete");
    btnDelete->setObjectName("actionButtonDelete");

    actionLayout->addWidget(btnAdd);
    actionLayout->addWidget(btnEdit);
    actionLayout->addWidget(btnDelete);
    actionLayout->addStretch();

    // 3. Tabela
    categoryTable = new QTableView();
    categoryTable->setModel(tableModel);
    categoryTable->setAlternatingRowColors(true);
    categoryTable->setShowGrid(false);
    categoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    categoryTable->setSelectionMode(QAbstractItemView::SingleSelection);
    categoryTable->verticalHeader()->setVisible(false);
    categoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


    tableModel->setColumnCount(2);
    tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    tableModel->setHeaderData(1, Qt::Horizontal, tr("Category Name"));
    categoryTable->setColumnHidden(0, true); 
    categoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    contentLayout->addLayout(headerLayout);
    contentLayout->addLayout(actionLayout);
    contentLayout->addWidget(categoryTable);

    connect(btnAdd, &QPushButton::clicked, this, &CategorySelectionView::onButtonAddClicked);
    connect(btnEdit, &QPushButton::clicked, this, &CategorySelectionView::onButtonEditClicked);
    connect(btnDelete, &QPushButton::clicked, this, &CategorySelectionView::onButtonDeleteClicked);
    connect(searchEdit, &QLineEdit::textChanged, this, &CategorySelectionView::onSearchTextChanged);
    connect(categoryTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &CategorySelectionView::onHeaderClicked);
}


void CategorySelectionView::setCategoryTabHeaders(const QVector<QStringList>& rows) const
{
    tableModel->removeRows(0, tableModel->rowCount());

    for (const QStringList& row : rows) {
        QList<QStandardItem*> items;
        for (const QString& value : row)
            items.append(new QStandardItem(value));
        tableModel->appendRow(items);
    }
}

int CategorySelectionView::getSelectedCategoryId() const
{
    QModelIndex index = categoryTable->currentIndex();
    if (!index.isValid()) return -1;
    bool ok;
    int id = tableModel->data(tableModel->index(index.row(), 0)).toInt(&ok);
    return ok ? id : -1;
}

void CategorySelectionView::showMessage(QString header, QString message, QString messageType)
{
    if (messageType == "error")
        QMessageBox::warning(this, header, message);
    else
        QMessageBox::information(this, header, message);
}

QString CategorySelectionView::getSearchText() const
{
    return searchEdit->text();
}
void CategorySelectionView::setupStyle()
{
    this->setStyleSheet(
        "QLabel#viewLabel { font-size: 24px; font-weight: bold; color: white; }"
        "QLineEdit{ padding: 8px; border: 1px solid #444444; border-radius: 5px; background-color: #2d2d2d; color: white; selection-background-color: #3498db; }"
        "QPushButton#actionButtonAdd { background-color: #27ae60; color: white; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton#actionButtonDelete { background-color: #c0392b; color: white; border-radius: 5px; padding: 8px 15px; }"
        "QPushButton#actionButton { background-color: #2980b9; color: white; border-radius: 5px; padding: 8px 15px; }"
        "QTableView { border: 1px solid #333333; background-color: #1e1e1e; color: white; gridline-color: #333333; selection-background-color: #3498db; selection-color: white; }"
        "QHeaderView::section { background-color: #2d2d2d; padding: 8px; border: 1px solid #333333; font-weight: bold; color: white; }"
        "QDialog, QMessageBox, QInputDialog { background-color: #1e1e1e; color: white; font-size: 14px; }"
        "QDialog QPushButton { background-color: #333333; color: white; border: 1px solid #444444; border-radius: 4px; padding: 6px 15px; }");
}

void CategorySelectionView::onButtonAddClicked() { emit addCategoryRequest(); }
void CategorySelectionView::onButtonEditClicked() { emit editCategoryRequest(); }
void CategorySelectionView::onButtonDeleteClicked() { emit deleteCategoryRequest(); }
void CategorySelectionView::onSearchTextChanged(const QString& text) { emit searchCategoryRequest(text); }
void CategorySelectionView::onHeaderClicked(int index) { emit columnSortRequest(index); }