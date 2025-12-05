#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    //sygnaly do oblsugi kontrolera
    void addTransactionRequested();
    void deleteTransactionRequested();

private slots:

    void onButtonAddTransactionClicked();
    void onButtonDeleteTransactionClicked();

private:
    Ui::MainWindow* ui;

    void setupConnections();
};



    

