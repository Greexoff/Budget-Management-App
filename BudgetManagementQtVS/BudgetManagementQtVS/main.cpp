#include "BudgetManagementQtVS.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BudgetManagementQtVS window;
    window.show();
    return app.exec();
}
