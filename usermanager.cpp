#include "usermanager.h"
#include "ui_usermanager.h"
#include <QInputDialog>
#include <QLineEdit>
#include <QPixmap>

usermanager::usermanager(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::usermanager)
{
    ui->setupUi(this);

    // Initialize database connection
    if (!db.connect())
    {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        return;
    }

    loadUsers();
}

usermanager::~usermanager()
{
    delete ui;
}


void usermanager::loadUsers()
{
    ui->usertable->setRowCount(0); // Clear the table

    QSqlQuery query = db.getAllUsers(); // Assuming you implement `getAllUsers()` in `Database`
    if (!query.isActive()) return;

    int row = 0;
    while (query.next())
    {
        ui->usertable->insertRow(row);

        ui->usertable->setItem(row, 0, new QTableWidgetItem(query.value("ID").toString()));
        ui->usertable->setItem(row, 1, new QTableWidgetItem(query.value("Username").toString()));
        ui->usertable->setItem(row, 2, new QTableWidgetItem(query.value("Email").toString()));
        ui->usertable->setItem(row, 3, new QTableWidgetItem(query.value("Password").toString()));

        ++row;
    }
}

void usermanager::on_adduserButton_clicked()
{
    QString email = QInputDialog::getText(this, "Add User", "Enter Email:");
    QString username = QInputDialog::getText(this, "Add User", "Enter Username:");
    QString password = QInputDialog::getText(this, "Add User", "Enter Password:");

    if (email.isEmpty() || username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be filled.");
        return;
    }

    if (db.createUser(email, username, password))
    {
        QMessageBox::information(this, "Success", "User added successfully.");
        loadUsers();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to add user.");
    }
}

void usermanager::on_deleteuserButton_clicked()
{
    QList<QTableWidgetItem*> selected = ui->usertable->selectedItems();
    if (selected.isEmpty())
    {
        QMessageBox::warning(this, "Error", "No user selected.");
        return;
    }

    int id = selected.first()->text().toInt();

    if (db.deleteUser(id)) { // Assuming `deleteUser(int id)` is implemented in `Database`
        QMessageBox::information(this, "Success", "User deleted successfully.");
        loadUsers();
    } else {
        QMessageBox::critical(this, "Error", "Failed to delete user.");
    }
}

void usermanager::on_edituserButton_clicked()
{
    QList<QTableWidgetItem*> selected = ui->usertable->selectedItems();
    if (selected.isEmpty())
    {
        QMessageBox::warning(this, "Error", "No user selected.");
        return;
    }

    int id = selected.first()->text().toInt();
    QString newEmail = QInputDialog::getText(this, "Edit User", "Enter New Email:");
    QString newUsername = QInputDialog::getText(this, "Edit User", "Enter New Username:");
    QString newPassword = QInputDialog::getText(this, "Edit User", "Enter New Password:");

    if (newEmail.isEmpty() || newUsername.isEmpty() || newPassword.isEmpty())
    {
        QMessageBox::warning(this, "Error", "All fields must be filled.");
        return;
    }

    if (db.updateUser(id, newEmail, newUsername, newPassword)) { // Assuming `updateUser()` is implemented in `Database`
        QMessageBox::information(this, "Success", "User updated successfully.");
        loadUsers();
    } else
    {
        QMessageBox::critical(this, "Error", "Failed to update user.");
    }
}





