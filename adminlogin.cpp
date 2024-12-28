#include "adminlogin.h"
#include "ui_adminlogin.h"
#include "database.h"
#include <QMessageBox>
#include <QPixmap>
#include "admindashboard.h"
adminlogin::adminlogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::adminlogin)
{
    ui->setupUi(this);
    QPixmap  pix(":/new/prefix1/src/person_28dp_E5EFC1_FILL0_wght400_GRAD0_opsz24.png") ;
    ui->usernamelabel->setPixmap(pix.scaled(40,40)) ;
    QPixmap pic2(":/new/prefix1/src/password_28dp_E5EFC1_FILL0_wght400_GRAD0_opsz24.png");
    ui->passwordlabel->setPixmap(pic2.scaled(40,40));
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

adminlogin::~adminlogin()
{
    delete ui;
}



void adminlogin::on_admin_login_button_clicked()
{

    QString adminUsername = ui->username_lineEdit->text();
    QString adminPassword = ui->password_lineEdit->text();

    Database db;
    if (!db.connect())
    {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        return;
    }

    if (db.validateAdmin(adminUsername, adminPassword))
    {
        QMessageBox::information(this, "Success", "Admin login successful.");

        admindashboard adminDashboard(this);
        adminDashboard.exec();
    } else {
        QMessageBox::critical(this, "Error", "Invalid admin credentials.");
    }
}

