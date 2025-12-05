#include "BudgetManagementQtVS.h"
#include "Model/DatabaseManager.h"
#include "View/View.h"
#include <QtWidgets/QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DatabaseManager::instance();
    BudgetManagementQtVS window;
    window.show();
    return app.exec();
}
