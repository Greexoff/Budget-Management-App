#pragma once

#include <QMainWindow>
#include <qstandarditemmodel.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class TransactionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TransactionWindow(QWidget* parent = nullptr);
    ~TransactionWindow();

    void setTransactionTabHeaders(const QVector<QStringList>& rows);

    int getSelectedTransactionId() const;

    void updateBudgetDisplay(double limit, double spent);

    void clearSearchEdit();
public slots:
    void showTransactionMessage(QString header, QString message, QString messageType);

signals:
    void addTransactionRequest();

    void deleteTransactionRequest();

    void showCategoriesRequest();

    void showFinancialAccountsRequest();

    void editTransactionRequest();

    void backToProfileRequested();

    void editBudgetRequest();

    void columnSortRequest(int columnId);

    void searchTextRequest(const QString& searchText);

private slots:
    void onButtonAddTransactionClicked();
    void onButtonDeleteTransactionClicked();
    void onButtonShowCategoriesClicked();
    void onButtonShowFinancialAccountsClicked();
    void onButtonEditTransactionClicked();
    void onButtonEditBudgetClicked();

    void onColumnHeaderClicked(int columnId);
    void searchTextChanged(QString searchText);

private:
    Ui::MainWindow* ui;                        
    QStandardItemModel* TableModel = nullptr;   

    void setupConnections();

    void initializeTransactionTable();

    void onButtonChangeProfileClicked();

};



    

