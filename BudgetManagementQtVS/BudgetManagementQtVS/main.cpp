#include "BudgetManagementQtVS.h"
#include "DatabaseManager.h"
#include "View.h"
#include <QtWidgets/QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DatabaseManager::instance();    //otwieranie bazy

    BudgetManagementQtVS window;
    window.show();
    return app.exec();
}
