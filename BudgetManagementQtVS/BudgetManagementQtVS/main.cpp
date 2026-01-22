
#include "Controller/AppController.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    DatabaseManager::instance();
    AppController appController;
    appController.start();
    return app.exec();
}
