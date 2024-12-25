#include "problemmanager.h"
#include "ui_problemmanager.h"
#include "database.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>

problemManager::problemManager(QWidget *parent)
    : QDialog(parent), ui(new Ui::problemManager)
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
    populateTable();
}

problemManager::~problemManager()
{
    delete ui;
}

void problemManager::populateTable()
{
    Database db;
    if (!db.connect())
    {
        QMessageBox::warning(this, "Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query = db.getAllProblems();
    ui->tableWidget->setRowCount(0);

    int row = 0;
    while (query.next())
    {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("ID").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("Title").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("Description").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("Input_Format").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("Output_Format").toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("Constraints").toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value("Example").toString()));
        row++;
    }


}


/*void problemManager::on_createButton_clicked()
{
    QString topic = QInputDialog::getText(this, "Create Problem", "Enter Topic Name:");
    if (topic.isEmpty()) return;

    QString title = QInputDialog::getText(this, "Create Problem", "Enter Problem Title:");
    if (title.isEmpty()) return;

    QString description = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Problem Description:");
    QString inputFormat = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Input Format:");
    QString outputFormat = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Output Format:");
    QString constraints = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Constraints:");
    QString example = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Example:");

    Database db;
    if (db.connect() && db.createProblem(title, description, inputFormat, outputFormat, constraints, example, topic)) {
        QMessageBox::information(this, "Success", "Problem created successfully!");
        populateTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to create problem.");
    }
    populateTable();

}
*/
void problemManager::on_createButton_clicked()
{
    // Step 1: Collect problem details
    QString topic = QInputDialog::getText(this, "Create Problem", "Enter Topic Name:");
    if (topic.isEmpty()) return;

    QString title = QInputDialog::getText(this, "Create Problem", "Enter Problem Title:");
    if (title.isEmpty()) return;

    QString description = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Problem Description:");
    QString inputFormat = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Input Format:");
    QString outputFormat = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Output Format:");
    QString constraints = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Constraints:");
    QString example = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Example:");

    // Step 2: Insert the problem into the database
    Database db;
    if (!db.connect()) {
        QMessageBox::warning(this, "Error", "Failed to connect to the database.");
        return;
    }

    int problemId = db.createProblemAndReturnId(title, description, inputFormat, outputFormat, constraints, example, topic);
    qDebug() << "Created Problem ID:" << problemId;
    if (problemId == -1) {
        QMessageBox::warning(this, "Error", "Failed to create problem.");
        return;
    }

    QMessageBox::information(this, "Success", "Problem created successfully! Now, add test cases.");

    // Step 3: Prompt the admin to add test cases
    while (true)
    {
        QString input = QInputDialog::getMultiLineText(this, "Add Test Case", "Enter Input for Test Case:");
        if (input.isEmpty()) break;

        QString expectedOutput = QInputDialog::getMultiLineText(this, "Add Test Case", "Enter Expected Output for Test Case:");
        if (expectedOutput.isEmpty()) break;

        double weight = QInputDialog::getDouble(this, "Add Test Case", "Enter Weight for Test Case:", 1.0, 0.0, 100.0, 2);

        Database db;
        if (db.connect() && db.addTestCase(problemId, input, expectedOutput, weight))
        {
            QMessageBox::information(this, "Success", "Test case added successfully!");
        } else
        {
            QMessageBox::warning(this, "Error", "Failed to add test case.");
        }

        // Ask if the admin wants to add another test case
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Add Another?", "Do you want to add another test case?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::No) break;
    }

    populateTable(); // Refresh the table with the updated problem list
}


void problemManager::on_deleteButton_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row == -1)
    {
        QMessageBox::warning(this, "Error", "Please select a problem to delete.");
        return;
    }

    int id = ui->tableWidget->item(row, 0)->text().toInt();
    qDebug() << "Deleting problem with ID:" << id;

    Database db;
    if (!db.connect()) {
        QMessageBox::warning(this, "Error", "Failed to connect to the database.");
        return;
    }

    // Delete related test cases first (if necessary)
    QSqlQuery deleteTestCasesQuery;
    deleteTestCasesQuery.prepare("DELETE FROM TestCases WHERE Problem_Id = ?");
    deleteTestCasesQuery.addBindValue(id);
    if (!deleteTestCasesQuery.exec()) {
        QMessageBox::warning(this, "Error", "Failed to delete related test cases.");
        qDebug() << "Error deleting test cases:" << deleteTestCasesQuery.lastError().text();
        return;
    }

    // Delete the problem itself
    if (db.deleteProblem(id)) {
        QMessageBox::information(this, "Success", "Problem deleted successfully!");
        populateTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete problem.");
        qDebug() << "Error deleting problem.";
    }
}



void problemManager::on_editButton_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row == -1)
    {
        QMessageBox::warning(this, "Error", "Please select a problem to edit.");
        return;
    }

    auto getCellText = [this, row](int column) -> QString {
        QTableWidgetItem *item = ui->tableWidget->item(row, column);
        return item ? item->text() : "";
    };

    int id = getCellText(0).toInt();
    QString title = QInputDialog::getText(this, "Edit Problem", "Enter new title:",
                                          QLineEdit::Normal, getCellText(1));
    QString description = QInputDialog::getText(this, "Edit Problem", "Enter new description:",
                                                QLineEdit::Normal, getCellText(2));
    QString inputFormat = QInputDialog::getText(this, "Edit Problem", "Enter new input format:",
                                                QLineEdit::Normal, getCellText(3));
    QString outputFormat = QInputDialog::getText(this, "Edit Problem", "Enter new output format:",
                                                 QLineEdit::Normal, getCellText(4));
    QString constraints = QInputDialog::getText(this, "Edit Problem", "Enter new constraints:",
                                                QLineEdit::Normal, getCellText(5));
    QString example = QInputDialog::getText(this, "Edit Problem", "Enter new example:",
                                            QLineEdit::Normal, getCellText(6));
    QString topic = QInputDialog::getText(this, "Edit Problem", "Enter new topic:",
                                          QLineEdit::Normal, getCellText(7));  // Topic column is the last

    Database db;
    if (db.connect() && db.updateProblem(id, title, description, inputFormat, outputFormat, constraints, example, topic))
    {
        QMessageBox::information(this, "Success", "Problem updated successfully!");
        populateTable();  // Refresh the table
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to update problem.");
    }
}




void problemManager::on_addTestCaseButton_clicked()
{
    int problemId = QInputDialog::getInt(this, "Add Test Case", "Enter Problem ID:");
    QString input = QInputDialog::getMultiLineText(this, "Add Test Case", "Enter Input:");
    QString expectedOutput = QInputDialog::getMultiLineText(this, "Add Test Case", "Enter Expected Output:");
    double weight = QInputDialog::getDouble(this, "Add Test Case", "Enter Weight:", 1.0);

    Database db;
    if (db.connect() && db.addTestCase(problemId, input, expectedOutput, weight))
    {
        QMessageBox::information(this, "Success", "Test case added successfully!");
    } else
    {
        QMessageBox::warning(this, "Error", "Failed to add test case.");
    }
}

