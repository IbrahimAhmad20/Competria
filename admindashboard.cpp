#include "admindashboard.h"
#include "ui_admindashboard.h"
#include <QMessageBox>
#include "problemmanager.h"
#include "usermanager.h"
#include <QPixmap>
//#include "usermanager.h"
//#include "submissionmonitor.h"

admindashboard::admindashboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admindashboard)
{

    ui->setupUi(this);
    this->setStyleSheet(
        "QDialog {"
        "    background-color: rgb(121, 122, 86);"
        "    color: white;"
        "    border-radius: 10px;"
        "    padding: 10px;"
        "}");
}

admindashboard::~admindashboard()
{
    delete ui;
}


void admindashboard::on_userManagerButton_clicked()
{

   usermanager userManagerDialog(this);
   userManagerDialog.exec();
}




void admindashboard::on_problemManagerButton_clicked()
{
    problemManager problemManagerDialog(this);
    problemManagerDialog.exec();

}




