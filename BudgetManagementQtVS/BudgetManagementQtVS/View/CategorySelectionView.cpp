#include "CategorySelectionView.h"

CategorySelectionView::CategorySelectionView(QWidget *parent)
	: QDialog(parent), ui(new Ui::CategorySelectionView)
{
	ui->setupUi(this);
	connectMethodToButton();
}
CategorySelectionView::~CategorySelectionView() {}

void CategorySelectionView::connectMethodToButton()
{
	connect(ui->selectCategoryButton, &QPushButton::clicked, this, &CategorySelectionView::selectCategoryButtonClicked);
	connect(ui->addCategoryButton, &QPushButton::clicked, this, &CategorySelectionView::addCategoryButtonClicked);
	connect(ui->deleteCategoryButton, &QPushButton::clicked, this, &CategorySelectionView::deleteCategoryButtonClicked);
	connect(ui->cancelButton, &QPushButton::clicked, this, &CategorySelectionView::cancelButtonClicked);
	//connect(ui.editCategoryButton, &QPushButton::clicked, this, &CategorySelectionView::editCategoryButtonClicked); Change this to editCategoryButton
}

void CategorySelectionView::setCategories(const QVector<Category>& categories)
{
    categoryId = categories;
    ui->categoriesList->clear();
    for (const auto& category : categoryId) 
    {
        ui->categoriesList->addItem(category.getCategoryName());
    }
}

void CategorySelectionView::selectCategoryButtonClicked()
{
    int row = ui->categoriesList->currentRow();
    if (row < 0 || row >= categoryId.size())
    {
        return;
    }

    emit selectRequestedCategory(categoryId[row].getCategoryId());
}

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

void CategorySelectionView::deleteCategoryButtonClicked()
{
    int row = ui->categoriesList->currentRow();
    if (row < 0 || row >= categoryId.size())
    {
        return;
    }

    emit deleteRequestedCategory(categoryId[row].getCategoryId());
}

void CategorySelectionView::cancelButtonClicked()
{
    reject();
}

