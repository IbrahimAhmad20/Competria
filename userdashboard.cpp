#include "userdashboard.h"
#include "ui_userdashboard.h"
#include <QMessageBox>
#include "solveproblemwindow.h"
#include "database.h"
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QProgressDialog>
userdashboard::userdashboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userdashboard)
{
    ui->setupUi(this);

    // Set window properties
    setWindowTitle("User Dashboard");
    setModal(true); // Optional: Make it modal
    setFixedSize(size()); // Optional: Prevent resizing

    // Load problems into the table view
    loadProblems();

    // Connect double-click event on the table to handle problem selection
    connect(ui->problemTable, &QTableWidget::cellDoubleClicked, this, &userdashboard::onProblemSelected);
}

userdashboard::~userdashboard()
{
    delete ui;
}

void userdashboard::loadProblems()
{
    if (!db.connect())
    {
        QMessageBox::warning(this, "Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query = db.getAllProblems();

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error", "Failed to fetch problems: " + query.lastError().text());
        return;
    }

    // Clear the table and set headers programmatically
    ui->problemTable->clear();
    QStringList headers = {"ID", "Title", "Constraints", "Topic"};
    ui->problemTable->setColumnCount(headers.size());
    ui->problemTable->setHorizontalHeaderLabels(headers);

    ui->problemTable->setRowCount(0);

    int row = 0;
    while (query.next())
    {
        ui->problemTable->insertRow(row);
        ui->problemTable->setItem(row, 0, new QTableWidgetItem(query.value("ID").toString()));
        ui->problemTable->setItem(row, 1, new QTableWidgetItem(query.value("Title").toString()));
        ui->problemTable->setItem(row, 2, new QTableWidgetItem(query.value("Constraints").toString()));
        ui->problemTable->setItem(row, 3, new QTableWidgetItem(query.value("Topic").toString()));
        row++;
    }

    ui->problemTable->resizeColumnsToContents();
}

void userdashboard::on_SolveChallenge_clicked()
{
    // Get the currently selected index in the table
    int selectedRow = ui->problemTable->currentRow();

    if (selectedRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a problem to solve.");
        return;
    }

    // Extract the problem ID from the selected row (assuming column 0 contains ID)
    int problemId = ui->problemTable->item(selectedRow, 0)->text().toInt();

    // Show a loading indicator while fetching data
    QProgressDialog *progressDialog = new QProgressDialog("Loading problem...", "Cancel", 0, 0, this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setCancelButton(nullptr); // Disable the cancel button
    progressDialog->show();

    // Load problem details in a separate thread
    QFuture<void> future = QtConcurrent::run([=]() {
        // Simulate a time-consuming operation
        QThread::sleep(2); // Replace with actual loading logic if necessary

        // Open the solve problem window in the main thread after data is ready
        QMetaObject::invokeMethod(this, [=]() {
            // Close and delete the loading dialog
            progressDialog->close();
            delete progressDialog;

            // Open the solve problem window
            solveproblemwindow *solveWindow = new solveproblemwindow(db, problemId, this);
            solveWindow->exec();
        });
    });
}


void userdashboard::onProblemSelected(int row, int column)
{
    Q_UNUSED(column);

    // Extract the problem ID from the selected row
    int problemId = ui->problemTable->item(row, 0)->text().toInt();
    QString title = ui->problemTable->item(row, 1)->text();

    QMessageBox::information(this, "Problem Selected",
                             QString("You selected Problem ID: %1\nTitle: %2").arg(problemId).arg(title));

    // Optionally, you could open the solve problem window here:
    // solveproblemwindow *solveWindow = new solveproblemwindow(db, problemId, this);
    // solveWindow->exec();
}
