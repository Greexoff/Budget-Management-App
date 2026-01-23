/**
 * @file main.cpp
 * @brief Main entry point of the Budget Management Application.
 */
#include "Controller/AppController.h"

 /**
  * @brief Main function.
  * Initializes the QApplication, Database Singleton, and starts the AppController.
  */
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    // Initialize Database Singleton
    DatabaseManager::instance();
    // Start Main Controller
    AppController appController;
    appController.start();
    return app.exec();
}
