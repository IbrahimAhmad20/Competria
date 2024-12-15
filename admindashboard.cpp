#include "admindashboard.h"
#include "ui_admindashboard.h"
#include <QMessageBox> // For potential notifications
#include "problemmanager.h"
#include "usermanager.h"
#include <QPixmap>// Include header for ProblemManager
//#include "usermanager.h"    // Include header for UserManager
//#include "submissionmonitor.h" // Include header for SubmissionMonitor

admindashboard::admindashboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admindashboard)
{

    ui->setupUi(this);

}

admindashboard::~admindashboard()
{
    delete ui;
}


void admindashboard::on_userManagerButton_clicked()
{
    // Open the User Manager dialog
   usermanager userManagerDialog(this);
   userManagerDialog.exec();
}

// AdminDashboard::on_submissionMonitorButton_clicked() {
    // Open the Submission Monitor dialog
   // SubmissionMonitor submissionMonitorDialog(this);
//submissionMonitorDialog.exec();
//}



void admindashboard::on_problemManagerButton_clicked()
{
    problemManager problemManagerDialog(this);
    problemManagerDialog.exec();

}

