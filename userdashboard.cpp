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
    setModal(true);
    setFixedSize(size());

    // Apply styles to the dialog and table widget
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

    // Ensure table resizes vertically to fit content
    ui->problemTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->problemTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Ensure the last column stretches
    ui->problemTable->horizontalHeader()->setStretchLastSection(true);

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

    QSqlQuery query = db.getProblemDetails();  // Use the new function

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error", "Failed to fetch problems: " + query.lastError().text());
        return;
    }

    // Clear the table and set headers programmatically
    ui->problemTable->clear();
    QStringList headers = {"ID", "Title", "Topic", "Constraints", "Description"};
    ui->problemTable->setColumnCount(headers.size());
    ui->problemTable->setHorizontalHeaderLabels(headers);

    ui->problemTable->setRowCount(0);

    int row = 0;
    while (query.next())
    {
        ui->problemTable->insertRow(row);
        ui->problemTable->setItem(row, 0, new QTableWidgetItem(query.value("ID").toString()));
        ui->problemTable->setItem(row, 1, new QTableWidgetItem(query.value("Title").toString()));
        ui->problemTable->setItem(row, 2, new QTableWidgetItem(query.value("Topic").toString()));
        ui->problemTable->setItem(row, 3, new QTableWidgetItem(query.value("Constraints").toString()));
        ui->problemTable->setItem(row, 4, new QTableWidgetItem(query.value("Description").toString()));
        row++;
    }

    // Adjust column widths and table properties
    ui->problemTable->resizeColumnsToContents();
    ui->problemTable->horizontalHeader()->setStretchLastSection(true);
    ui->problemTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

}
