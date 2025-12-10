#pragma once

#include <QMainWindow>
#include <qstandarditemmodel.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Main application window for budget management
 *
 * The MainWindow provides the primary interface for viewing and managing
 * transactions. It displays transactions in a table view and provides
 * buttons for adding/deleting transactions and managing categories.
 */
class TransactionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TransactionWindow(QWidget* parent = nullptr);
    ~TransactionWindow();

    /**
     * @brief Populates the transaction table with data
     * @param rows Vector of string lists, each representing a transaction row
     */
    void setTransactionRows(const QVector<QStringList>& rows);

    /**
     * @brief Gets the ID of the currently selected transaction
     * @return int Transaction ID, or -1 if no transaction selected
     */
    int getSelectedTransactionId() const; 
public slots:
    void showTransactionMessage(QString header, QString message, QString messageType);
    QString getTransactionNameFromInput(bool& correctData, const QString& defaultValue = "");
    double getTransactionAmountFromInput(bool& correctData, double defaultValue = 0.0);
    QString getTransactionDescriptionFromInput(bool& correctData, const QString& defaultValue = "");

signals:
    /**
     * @brief Emitted when user requests to add a new transaction
     */
    void addTransactionRequest();

    /**
     * @brief Emitted when user requests to delete selected transaction
     */
    void deleteTransactionRequest();

    /**
     * @brief Emitted when user requests to manage categories
     */
    void showCategoriesRequest();

    void showFinancialAccountsRequest();

    void editTransactionRequest();

    void backToProfileRequested();

private slots:
    void onButtonAddTransactionClicked();
    void onButtonDeleteTransactionClicked();
    void onButtonManageCategoriesClicked();
    void onButtonManageFinancialAccountsClicked();
    void onButtonEditTransactionClicked();

private:
    Ui::MainWindow* ui;                         ///< Pointer to UI elements
    QStandardItemModel* TableModel = nullptr;   ///< Data model for transaction table

    /**
     * @brief Sets up connections for UI buttons
     */
    void setupConnections();

    /**
     * @brief Initializes the transaction table model
     *
     * Sets up table columns, headers, and display properties.
     */
    void initializeTransactionTable();

    void onButtonChangeProfileClicked();

};



    

