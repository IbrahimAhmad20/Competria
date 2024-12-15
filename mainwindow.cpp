#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adminlogin.h"
#include "userlogin.h"
#include "createuser.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_admininterfacebutton_clicked()
{
    adminlogin adminLoginDialog(this);
    adminLoginDialog.exec(); // Opens the admin login dialog
}

void MainWindow::on_userinterfacebutton_clicked()
{
    userlogin userLoginDialog(this);
    userLoginDialog.exec(); // Opens the user login dialog
}





