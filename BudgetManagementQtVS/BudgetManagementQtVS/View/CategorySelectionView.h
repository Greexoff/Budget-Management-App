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

    void setSelectCategoryButtonVisible(bool visible);
signals:
    void selectRequestedCategory(int categoryId);
    void addRequestedCategory(const QString& categoryName);
    void deleteRequestedCategory(int categoryId);
    //void editRequestedCategory(int categoryId); add this method later

private slots:
    void selectCategoryButtonClicked();
    void addCategoryButtonClicked();
    void deleteCategoryButtonClicked();
    void cancelButtonClicked();
    //void editCategoryButtonClicked();

private:
	Ui::CategorySelectionView* ui;
    QVector<Category> categoryId;
    void connectMethodToButton();
};

