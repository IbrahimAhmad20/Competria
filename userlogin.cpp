#include "userlogin.h"
#include "ui_userlogin.h"
#include "database.h"
#include <QMessageBox>
#include "createuser.h"
#include "userdashboard.h"
#include "ui_userdashboard.h"

userlogin::userlogin(QWidget *parent) : QDialog(parent), ui(new Ui::userlogin)
{
    ui->setupUi(this);
}

userlogin::~userlogin()
{
    delete ui;
}

void userlogin::on_login_button_clicked()
{
    QString username = ui->username_lineEdit->text();
    QString password = ui->password_lineEdit_2->text();

    Database db;
    if (!db.connect())
    {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        return;
    }

    if (db.validateUser(username, password))
    {
        userdashboard *userdashboardDialog = new userdashboard(this);
        this->close();
        userdashboardDialog->exec();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Invalid user credentials.");
    }
}
void userlogin::on_signup_button_clicked()
{
    createuser createUserDialog(this);
    createUserDialog.exec();
}
