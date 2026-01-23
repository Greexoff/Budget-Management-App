/**
 * @file ChartsController.h
 * @brief Header file for the Charts Controller.
 */
#pragma once
#include <Controller/BaseController.h>

#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>

#include <View/TransactionWindowView.h>
#include "View/ChartsDialogView.h"
#include <QPointer>

 /**
  * @class ChartsController
  * @brief Manages data aggregation and updating of the Charts View.
  */
class ChartsController : public BaseController
{
    Q_OBJECT

public:
    /** @brief Constructs the controller. */
    ChartsController(TransactionRepository& transRepo, CategoryRepository& catRepo, QObject* parent = nullptr);

    /** @brief Starts the controller (refreshes data). */
    void run() override;

    /** @brief Returns the view. */
    QPointer<ChartsView> getView() const;

public slots:
    /** @brief Recalculates statistics and updates the view. */
    void refreshData();

private:
    QPointer<ChartsView> view;
    TransactionRepository& transactionRepository;
    CategoryRepository& categoryRepository;

};