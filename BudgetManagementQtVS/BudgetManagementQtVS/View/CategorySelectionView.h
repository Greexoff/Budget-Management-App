/**
 * @file CategorySelectionView.h
 * @brief Header file for the Category Selection View widget.
 */
#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>

/**
 * @class CategorySelectionView
 * @brief Manages the display and interaction logic for transaction categories.
 */
class CategorySelectionView : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Applies CSS stylesheets to the widget components.
     */
    void setupStyle();
    /**
     * @brief Constructs the CategorySelectionView.
     * @param parent The parent widget.
     */
    explicit CategorySelectionView(QWidget* parent = nullptr);
    /**
     * @brief Default destructor.
     */
    ~CategorySelectionView() = default;

    /**
     * @brief Populates the category table with data.
     * @param rows A vector of string lists containing category data.
     */
    void setCategoryTabHeaders(const QVector<QStringList>& rows) const;
    /**
     * @brief Retrieves the ID of the currently selected category.
     * @return The selected category ID, or -1 if none is selected.
     */
    int getSelectedCategoryId() const;
    /**
     * @brief Displays a standardized message box to the user.
     * @param header The title of the message box.
     * @param message The body text of the message.
     * @param messageType The type of message ("error" or "info").
     */
    void showMessage(QString header, QString message, QString messageType);
    /**
     * @brief Retrieves the current text from the search bar.
     * @return The search string.
     */
    QString getSearchText() const;
    /**
     * @brief Emits the refresh request signal.
     */
    void refreshView() { emit refreshRequest(); }
signals:
    /** @brief Emitted when the "Add" button is clicked. */
    void addCategoryRequest();
    /** @brief Emitted when the "Delete" button is clicked. */
    void deleteCategoryRequest();
    /** @brief Emitted when the "Edit" button is clicked. */
    void editCategoryRequest();
    /** @brief Emitted when the search text changes. */
    void searchCategoryRequest(const QString& searchText);
    /** @brief Emitted when a table column header is clicked. */
    void columnSortRequest(int columnId);
    /** @brief Emitted when a manual view refresh is requested. */
    void refreshRequest();

private slots:

    void onButtonAddClicked();
    void onButtonEditClicked();
    void onButtonDeleteClicked();
    void onSearchTextChanged(const QString& text);
    void onHeaderClicked(int index);

private:

    QStandardItemModel* tableModel;
    QTableView* categoryTable;
    QLineEdit* searchEdit;
    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnDelete;
    /** @brief Initializes the UI layout and widgets. */
    void setupUI();
};
