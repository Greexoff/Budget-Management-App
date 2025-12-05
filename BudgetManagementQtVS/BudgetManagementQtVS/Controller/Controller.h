#pragma once

#include "Model/Transaction.h"
#include "Model/TransactionRepository.h"
#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDate>



class MainWindow;


class TransactionController : public QObject
{
    Q_OBJECT

public:
    explicit TransactionController(MainWindow* view, QObject* parent = nullptr);

private slots:
    void handleAddTransactionRequested();
    void handleDeleteTransactionRequested();

private:
    MainWindow* mainWindowView;
    TransactionRepository transcationRepository;

    void refreshView();
};
