#include "Controller.h"
#include "View/MainWindowView.h"  
#include <QInputDialog>
#include <QMessageBox>

TransactionController::TransactionController(MainWindow* view, QObject* parent)
    : QObject(parent),
    mainWindowView(view),
    transcationRepository()
{

    connect(mainWindowView, &MainWindow::addTransactionRequested, this, &TransactionController::handleAddTransactionRequested);

    connect(mainWindowView, &MainWindow::deleteTransactionRequested,this, &TransactionController::handleDeleteTransactionRequested);

    refreshView();
}

void TransactionController::refreshView()
{
    QVector<Transaction> all = transcationRepository.getAll();
    QVector<QStringList> rows;

    for (const auto& transaction : all)
    {
        QStringList row;
        row << QString::number(transaction.getTransactionId())
            << transaction.getTransactionName()
            << transaction.getTransactionDate().toString("yyyy-MM-dd")
            << transaction.getTransactionDescription()
            << QString::number(transaction.getTransactionAmount(), 'f', 2);

        QString typeStr = (transaction.getTransactionAmount() > 0.0) ? "Wydatek" : "Przychód";
        row << typeStr;

        QString categoryText = QString("Brak");
        row << categoryText;

        rows.append(row);
    }

    mainWindowView->setTransactionRows(rows);
}

void TransactionController::handleAddTransactionRequested()
{
    bool correctDataType = false;

    QString name = QInputDialog::getText(
        mainWindowView,
        QObject::tr("Nowa transakcja"),
        QObject::tr("Nazwa:"),
        QLineEdit::Normal,
        "",
        &correctDataType
    );
    if (!correctDataType || name.trimmed().isEmpty())
        return;

    double amount = QInputDialog::getDouble(
        mainWindowView,
        QObject::tr("Nowa transakcja"),
        QObject::tr("Kwota:"),
        0.0,
        -1e9,
        1e9,
        2,
        &correctDataType
    );
    if (!correctDataType)
        return;

    QString description = QInputDialog::getText(
        mainWindowView,
        QObject::tr("Nowa transakcja"),
        QObject::tr("Opis:"),
        QLineEdit::Normal,
        "",
        &correctDataType
    );
    if (!correctDataType)
        return;


    TransactionType type = (amount >= 0.0) ? INCOME : EXPENSE;
    int categoryId = 0; // TODO: jak dodamy kategorie


    Transaction transaction(
        0,                     
        name,
        QDate::currentDate(),
        description,
        amount,
        type,
        categoryId
    );

    if (!transcationRepository.add(transaction))
    {
        QMessageBox::warning(
            mainWindowView,
            QObject::tr("Błąd"),
            QObject::tr("Nie udało się dodać transakcji do bazy.")
        );
        return;
    }

    refreshView();
}

void TransactionController::handleDeleteTransactionRequested()
{
    int id = mainWindowView->selectedTranstacionId();
    if (id < 0)
    {
        QMessageBox::information(
            mainWindowView,
            QObject::tr("Usuń transakcję"),
            QObject::tr("Nie wybrano żadnej transakcji.")
        );
        return;
    }

    if (!transcationRepository.removeById(id))
    {
        QMessageBox::warning(
            mainWindowView,
            QObject::tr("Błąd"),
            QObject::tr("Nie udało się usunąć transakcji z bazy.")
        );
        return;
    }

    refreshView();
}
