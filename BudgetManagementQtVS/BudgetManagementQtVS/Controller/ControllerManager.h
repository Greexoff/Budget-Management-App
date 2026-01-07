#pragma once
#include <QObject>
#include <Controller/UserController.h>
#include <Controller/ProfileController.h>
#include <Controller/TransactionController.h>
#include <Controller/CategorySelectionController.h>
#include <Controller/FinancialAccountSelectionController.h>
#include "Controller/ChartsController.h"

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
        FinancialAccountController& financialAccountControllerRef,
		ChartsController& chartsControllerRef,
        QObject* parent = nullptr);

private:
    UserController& userController;
    ProfileController& profileController;
    TransactionController& transactionController;
    CategoryController& categoryController;
    FinancialAccountController& financialAccountController;
	ChartsController& chartsController;
};