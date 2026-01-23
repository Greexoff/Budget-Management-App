#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>

class CategorySelectionView : public QWidget
{
    Q_OBJECT

public:
    void setupStyle();
    explicit CategorySelectionView(QWidget* parent = nullptr);
    ~CategorySelectionView() = default;


    void setCategoryTabHeaders(const QVector<QStringList>& rows) const;
    int getSelectedCategoryId() const;
    void showMessage(QString header, QString message, QString messageType);
    
    QString getSearchText() const;

    void refreshView() { emit refreshRequest(); }
signals:

    void addCategoryRequest();
    void deleteCategoryRequest();
    void editCategoryRequest();
    void searchCategoryRequest(const QString& searchText);
    void columnSortRequest(int columnId);
    void refreshRequest();

private slots:

    void onButtonAddClicked();
    void onButtonEditClicked();
    void onButtonDeleteClicked();
    void onSearchTextChanged(const QString& text);
    void onHeaderClicked(int index);

private:

    QStandardItemModel* tableModel;
    QTableView* categoryTable;
    QLineEdit* searchEdit;
    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnDelete;

    void setupUI();
};
