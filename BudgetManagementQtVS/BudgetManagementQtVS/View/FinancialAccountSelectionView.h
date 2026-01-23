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

class FinancialAccountSelectionView : public QWidget
{
    Q_OBJECT

public:
    explicit FinancialAccountSelectionView(QWidget* parent = nullptr);

    void setAccountTabHeaders(const QVector<QStringList>& rows) const;
    int getSelectedAccountId() const;
    void showMessage(QString header, QString message, QString messageType);
    QString getSearchText() const;
    void refreshView() { emit refreshRequest(); }

signals:

    void addAccountRequest(const QString& name, const QString& type, double balance);
    void deleteAccountRequest();
    void editAccountRequest(int id, const QString& name, const QString& type, double balance);
    void searchAccountRequest(const QString& searchText);
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
    QTableView* accountTable;
    QLineEdit* searchEdit;
    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnDelete;

    void setupUI();
    void setupStyle();
};