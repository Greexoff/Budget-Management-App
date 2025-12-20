#pragma once

#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>
#include "ui_CategorySelectionView.h"
#include "Model/Category.h"

class CategorySelectionView : public QDialog
{
	Q_OBJECT

public:
	explicit CategorySelectionView(QWidget *parent = nullptr);
	~CategorySelectionView();

    void setCategories(const QVector<Category>& categories);

    int getSelectedCategoryId() const { return selectedCategoryId; }

    void clearSearchLineEdit();

public slots:
    void showCategoryMessage(QString header, QString message, QString messageType);
signals:

    void selectRequestedCategory(int categoryId);

    void addRequestedCategory(const QString& categoryName);

    void deleteRequestedCategory(int categoryId);

    void editRequestedCategory(int categoryId, const QString& newName);

    void searchTextRequest(const QString& searchText);

    void columnSortRequest(int columnId);

private slots:
    void addCategoryButtonClicked();
    void deleteCategoryButtonClicked();
    void cancelButtonClicked();
    void editCategoryButtonClicked();
    void searchTextChanged(const QString& searchText);
    void onColumnHeaderClicked(int columnId);

private:
	Ui::CategorySelectionView* ui;      
    QVector<Category> categoryId;       
    int selectedCategoryId = -1;    

    void connectMethodToButton();
    void setUpTable();
};

