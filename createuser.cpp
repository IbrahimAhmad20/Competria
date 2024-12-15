
#include "createuser.h"
#include "ui_createuser.h"
#include "database.h"
#include <QMessageBox>
#include <QPixmap>

createuser::createuser(QWidget *parent) : QDialog(parent), ui(new Ui::createuser)
{

    ui->setupUi(this);
    QPixmap email(":/new/prefix1/src/mail_30dp_6B2346_FILL0_wght400_GRAD0_opsz24.png");
    QPixmap username(":/new/prefix1/src/person_28dp_E5EFC1_FILL0_wght400_GRAD0_opsz24.png");
    QPixmap password(":/new/prefix1/src/password_28dp_E5EFC1_FILL0_wght400_GRAD0_opsz24.png");

    ui->emaillabel->setPixmap(email.scaled(40,40));
    ui->passwordlabel->setPixmap(password.scaled(40,40));
    //ui->confirmpasswordlabel->setPixmap(password.scaled(40,40));
    ui->usernamelabel->setPixmap(username.scaled(40,40));
}

createuser::~createuser()
{
    delete ui;
}

void createuser::on_create_user_button_clicked()
{
    QString email = ui->email_lineEdit->text();
    QString username = ui->username_lineEdit->text();
    QString password = ui->password_lineEdit->text();
    QString confirmPassword = ui->confirmpass_lineEdit->text();

    if (password != confirmPassword)
    {
        QMessageBox::critical(this, "Error", "Passwords do not match.");
        return;
    }

    Database db;
    if (!db.connect())
    {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
        return;
    }

    if (db.createUser(email, username, password))
    {
        QMessageBox::information(this, "Success", "User account created successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to create user account.");
    }
}
