#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include "database.h"

namespace Ui
{
class usermanager;
}

class usermanager : public QDialog
{
    Q_OBJECT

public:
    explicit usermanager(QWidget *parent = nullptr);
    ~usermanager();

private slots:
    void loadUsers(); // Load users from the database into the table

    void on_adduserButton_clicked();// Handle the Add button

    void on_deleteuserButton_clicked(); // Handle the Delete button

    void on_edituserButton_clicked();

private:
    Ui::usermanager *ui;
    Database db;
    void setupConnections();
};

#endif // USERMANAGER_H
