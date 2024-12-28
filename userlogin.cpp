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
    QPixmap  pix(":/new/prefix1/src/person_28dp_E5EFC1_FILL0_wght400_GRAD0_opsz24.png") ;
    ui->username_label->setPixmap(pix.scaled(40,40)) ;
    QPixmap pic2(":/new/prefix1/src/password_28dp_E5EFC1_FILL0_wght400_GRAD0_opsz24.png");
    ui->password_label->setPixmap(pic2.scaled(40,40));
    this->setStyleSheet(
        "QDialog {"
        "     background-color: rgb(17, 45, 50);"
        "    color: white;"
        "    border-radius: 10px;"
        "    padding: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #9a76b1;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #6a4a75;"
        "}");

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
