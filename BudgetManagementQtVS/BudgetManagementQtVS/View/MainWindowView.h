#pragma once

#include <QMainWindow>
#include <qstandarditemmodel.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void setTransactionRows(const QVector<QStringList>& rows);    

    int selectedTranstacionId() const; //jak sie kliknie na transakcje w tabli to zwraca jej id

signals:
    //sygnaly do oblsugi kontrolera
    void addTransactionRequested();
    void deleteTransactionRequested();

private slots:

    void onButtonAddTransactionClicked();
    void onButtonDeleteTransactionClicked();

private:
    Ui::MainWindow* ui;
    QStandardItemModel* TableModel = nullptr;

    void setupConnections();
    void setupTableModel();         //Setupwoanie kolumn w naszej tabeli
};



    

