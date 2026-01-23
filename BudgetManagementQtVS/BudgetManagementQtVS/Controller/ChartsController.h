#pragma once
#include <Controller/BaseController.h>

#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>

#include <View/TransactionWindowView.h>
#include "View/ChartsDialogView.h"
#include <QPointer>


class ChartsController : public BaseController
{
    Q_OBJECT

public:
    ChartsController(TransactionRepository& transRepo, CategoryRepository& catRepo, QObject* parent = nullptr);
    void run() override;

    QPointer<ChartsView> getView() const;

public slots:
    void refreshData();

private:
    QPointer<ChartsView> view;
    TransactionRepository& transactionRepository;
    CategoryRepository& categoryRepository;

};