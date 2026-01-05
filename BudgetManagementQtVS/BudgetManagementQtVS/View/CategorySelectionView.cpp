#include "CategorySelectionView.h"

CategorySelectionView::CategorySelectionView(QWidget *parent)
	: QDialog(parent), ui(new Ui::CategorySelectionView)
{
	ui->setupUi(this);
    setWindowTitle("Browse category");
    setUpTable();
	connectMethodToButton();
}

CategorySelectionView::~CategorySelectionView() 
{
    delete ui;
}

//----------------Setting connection (button-method)-------------------------------------------------

void CategorySelectionView::connectMethodToButton()
{
    connect(ui->addCategoryButton, &QPushButton::clicked, this, &CategorySelectionView::addCategoryButtonClicked);
    connect(ui->deleteCategoryButton, &QPushButton::clicked, this, &CategorySelectionView::deleteCategoryButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &CategorySelectionView::cancelButtonClicked);
    connect(ui->editCategoryButton, &QPushButton::clicked, this, &CategorySelectionView::editCategoryButtonClicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &CategorySelectionView::searchTextChanged);

    QHeaderView* header = ui->categoryTable->horizontalHeader();
    connect(header, &QHeaderView::sectionClicked, this, &CategorySelectionView::onColumnHeaderClicked);
}


//----------------Setting up view-------------------------------------------------


//Method that sets up table where all catergories are shown
void CategorySelectionView::setUpTable()
{
    ui->categoryTable->setColumnCount(1);
    ui->categoryTable->setHorizontalHeaderLabels({ "Name" });
    ui->categoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->categoryTable->verticalHeader()->setVisible(false);
    ui->categoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->categoryTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

//Method responsible for filling table with categories
void CategorySelectionView::setCategories(const QVector<Category>& categories)
{
    categoryId = categories;
    ui->categoryTable->setRowCount(0);
    for (const auto& category : categoryId) 
    {
        int row = ui->categoryTable->rowCount();
        ui->categoryTable->insertRow(row);
        ui->categoryTable->setItem(row, 0, new QTableWidgetItem(category.getCategoryName()));
    }
}


//----------------Pressing buttons actions-------------------------------------------------


//Method that notices clicking on add button
void CategorySelectionView::addCategoryButtonClicked()
{
    bool ok = false;
    QString name = QInputDialog::getText(this, tr("New category"), tr("Category name:"), QLineEdit::Normal, "", &ok);
    if (!ok || name.trimmed().isEmpty())
    {
        return;
    }

    emit addRequestedCategory(name);
}

//Method that notices clicking on edit button
void CategorySelectionView::editCategoryButtonClicked()
{
    int row = ui->categoryTable->currentRow();

    if (row < 0 || row >= categoryId.size()) {
        return;
    }

    Category currentCat = categoryId[row];

    if (currentCat.getCategoryId() == 1) {
        showCategoryMessage(tr("Edit category"), tr("Cannot edit default category."), "error");
        return;
    }

    bool ok = false;

    QString newName = QInputDialog::getText(
        this,
        tr("Edit category"),
        tr("New name:"),
        QLineEdit::Normal,
        currentCat.getCategoryName(),
        &ok
    );

    if (ok && !newName.trimmed().isEmpty()) {
        emit editRequestedCategory(currentCat.getCategoryId(), newName);
    }
}

//Method that notices clicking on delete button
void CategorySelectionView::deleteCategoryButtonClicked()
{
    int row = ui->categoryTable->currentRow();
    if (row < 0 || row >= categoryId.size())
    {
        return;
    }

    emit deleteRequestedCategory(categoryId[row].getCategoryId());
}

//Method that notices clicking on cancel button
void CategorySelectionView::cancelButtonClicked()
{
    reject();
}

//Method that clears search bar while reentering view
void CategorySelectionView::clearSearchLineEdit()
{
    ui->searchLineEdit->clear();
}

//Method that passes text inserted in search bar
void CategorySelectionView::searchTextChanged(const QString& searchText)
{
    emit searchTextRequest(searchText);
}

//Method that notices clicking on column header
void CategorySelectionView::onColumnHeaderClicked(int columnId)
{
    emit columnSortRequest(columnId);
}

//Method responsible for displaying error if inserted data is incorrect/is lacking 
void CategorySelectionView::showCategoryMessage(QString header, QString message, QString messageType)
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