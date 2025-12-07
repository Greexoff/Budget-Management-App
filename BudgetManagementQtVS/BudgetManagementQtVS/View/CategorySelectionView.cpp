#include "CategorySelectionView.h"

/**
 * @brief Constructs the category selection dialog
 * @param parent Parent widget
 */
CategorySelectionView::CategorySelectionView(QWidget *parent)
	: QDialog(parent), ui(new Ui::CategorySelectionView)
{
	ui->setupUi(this);
    setWindowTitle("Browse category");
	connectMethodToButton();
}

/**
 * @brief Destructor - cleans up UI resources
 */
CategorySelectionView::~CategorySelectionView() 
{
    delete ui;
}

/**
 * @brief Sets up signal-slot connections for UI buttons
 */
void CategorySelectionView::connectMethodToButton()
{
	connect(ui->selectCategoryButton, &QPushButton::clicked, this, &CategorySelectionView::selectCategoryButtonClicked);
	connect(ui->addCategoryButton, &QPushButton::clicked, this, &CategorySelectionView::addCategoryButtonClicked);
	connect(ui->deleteCategoryButton, &QPushButton::clicked, this, &CategorySelectionView::deleteCategoryButtonClicked);
	connect(ui->cancelButton, &QPushButton::clicked, this, &CategorySelectionView::cancelButtonClicked);
	//connect(ui.editCategoryButton, &QPushButton::clicked, this, &CategorySelectionView::editCategoryButtonClicked); Add logic to it later
}

/**
 * @brief Populates the category list with provided categories
 * @param categories Vector of categories to display
 */
void CategorySelectionView::setCategories(const QVector<Category>& categories)
{
    categoryId = categories;
    ui->categoriesList->clear();
    for (const auto& category : categoryId) 
    {
        ui->categoriesList->addItem(category.getCategoryName());
    }
}

/**
 * @brief Handles click on the "Select" button
 */
void CategorySelectionView::selectCategoryButtonClicked()
{
    int row = ui->categoriesList->currentRow();
    if (row < 0 || row >= categoryId.size())
    {
        return;
    }

    selectedCategoryId = categoryId[row].getCategoryId();
    emit selectRequestedCategory(categoryId[row].getCategoryId());
    accept();
}

/**
 * @brief Handles click on the "Add" button
 */
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

/**
 * @brief Handles click on the "Delete" button
 */
void CategorySelectionView::deleteCategoryButtonClicked()
{
    int row = ui->categoriesList->currentRow();
    if (row < 0 || row >= categoryId.size())
    {
        return;
    }

    emit deleteRequestedCategory(categoryId[row].getCategoryId());
}

/**
 * @brief Handles click on the "Cancel" button
 */
void CategorySelectionView::cancelButtonClicked()
{
    reject();
}

/**
 * @brief Controls visibility of the selection button
 * @param visible True to show, false to hide
 */
void CategorySelectionView::setSelectCategoryButtonVisible(bool visible)
{
    if (ui->selectCategoryButton) {
        ui->selectCategoryButton->setVisible(visible);
    }
}
void CategorySelectionView::displayCategoryMessage(QString header, QString message, QString messageType)
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
