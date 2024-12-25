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
    this->setStyleSheet(
        "QDialog {"
        "    background-color: #2b2b2b;"
        "    color: white;"
        "    border-radius: 10px;"
        "    padding: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #9a76b1;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #6a4a75;"
        "}"
        "QTableWidget {"
        "    background-color: #3c3f41;"
        "    alternate-background-color: #4a4a4a;"
        "    gridline-color: #5a5a5a;"
        "    color: white;"
        "    border: 1px solid #5c5c5c;"
        "    font-size: 14px;"
        "}"
        "QHeaderView::section {"
        "    background-color: #4a4a4a;"
        "    color: white;"
        "    padding: 4px;"
        "    border: none;"
        "    font-weight: bold;"
        "    text-align: left;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #7b5c8e;"
        "    color: white;"
        "}"
        );
    ui->usertable->verticalHeader()->setDefaultSectionSize(40); // Adjust height
    ui->usertable->horizontalHeader()->setStretchLastSection(true); // Stretch last column
    for (int i = 0; i < ui->usertable->rowCount(); ++i) {
        for (int j = 0; j < ui->usertable->columnCount(); ++j) {
            if (ui->usertable->item(i, j)) {
                ui->usertable->item(i, j)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }

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
    ui->usertable->setRowCount(0);

    QSqlQuery query = db.getAllUsers();
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

    if (db.updateUser(id, newEmail, newUsername, newPassword)) {
        QMessageBox::information(this, "Success", "User updated successfully.");
        loadUsers();
    } else
    {
        QMessageBox::critical(this, "Error", "Failed to update user.");
    }
}





