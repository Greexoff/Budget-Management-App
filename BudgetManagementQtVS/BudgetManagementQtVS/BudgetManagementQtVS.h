#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BudgetManagementQtVS.h"

class BudgetManagementQtVS : public QMainWindow
{
    Q_OBJECT

public:
    BudgetManagementQtVS(QWidget *parent = nullptr);
    ~BudgetManagementQtVS();

private:
    Ui::BudgetManagementQtVSClass ui;
};

