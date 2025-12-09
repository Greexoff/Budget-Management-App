#pragma once
#include <QObject>
#include <Controller/UserController.h>
#include <Controller/ProfileController.h>
#include <Controller/TransactionController.h>
#include <Controller/CategorySelectionController.h>

#include <Model/TransactionBuilder.h>

class ControllerManager : public QObject
{
    Q_OBJECT
public:
    ControllerManager(
        UserController& userControllerRef,
        ProfileController& profileControllerRef,
        TransactionController& transactionControllerRef,
        CategoryController& categoryControllerRef,
        QObject* parent = nullptr);

private:
    UserController& userController;
    ProfileController& profileController;
    TransactionController& transactionController;
    CategoryController& categoryController;
};