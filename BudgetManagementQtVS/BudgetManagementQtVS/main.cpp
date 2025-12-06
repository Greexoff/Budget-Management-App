#include "Controller/Controller.h"
#include "View/MainWindowView.h"
#include <QtWidgets/QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DatabaseManager::instance();

    TransactionController controller;
    controller.run();

    return app.exec();
}
