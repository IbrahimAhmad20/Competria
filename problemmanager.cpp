/*#include "problemmanager.h"
#include "ui_problemmanager.h"
#include "database.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QPixmap>
#include <QCryptographicHash>


problemManager::problemManager(QWidget *parent)
    : QDialog(parent), ui(new Ui::problemManager)
{
    ui->setupUi(this);
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
    ui->tableWidget->setRowCount(0); // Clear existing rows

    int row = 0;
    while (query.next())
    {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("ID").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("Title").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("Description").toString()));
        row++;
    }
}

void problemManager::on_createButton_clicked()
{

    QString title = QInputDialog::getText(this, "Create Problem", "Enter Problem Title:");
    if (title.isEmpty()) return;

    QString description = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Problem Description:");
    QString inputFormat = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Input Format:");
    QString outputFormat = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Output Format:");
    QString sampleInputs = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Sample Inputs:");
    QString sampleOutputs = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Sample Outputs:");

    // Insert into database
    QSqlQuery query;
    query.prepare("INSERT INTO problems (title, description, input_format, output_format, sample_inputs, sample_outputs) "
                  "VALUES (:title, :description, :input_format, :output_format, :sample_inputs, :sample_outputs)");
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":input_format", inputFormat);
    query.bindValue(":output_format", outputFormat);
    query.bindValue(":sample_inputs", sampleInputs);
    query.bindValue(":sample_outputs", sampleOutputs);

    Database db;
    if (db.connect() && db.createProblem(title, description))
    {
        QMessageBox::information(this, "Success", "Problem created successfully!");
        populateTable();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to create problem.");
    }
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

    Database db;
    if (db.connect() && db.deleteProblem(id)) {
        QMessageBox::information(this, "Success", "Problem deleted successfully!");
        populateTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete problem.");
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

    int id = ui->tableWidget->item(row, 0)->text().toInt();
    QString title = QInputDialog::getText(this, "Edit Problem", "Enter new title:", QLineEdit::Normal, ui->tableWidget->item(row, 1)->text());
    QString description = QInputDialog::getText(this, "Edit Problem", "Enter new description:", QLineEdit::Normal, ui->tableWidget->item(row, 2)->text());

    Database db;
    if (db.connect() && db.updateProblem(id, title, description))
    {
        QMessageBox::information(this, "Success", "Problem updated successfully!");
        populateTable();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to update problem.");
    }

}

void problemManager::on_viewButton_clicked()
{
    populateTable();
}
*/
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
    ui->tableWidget->setRowCount(0); // Clear existing rows

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

void problemManager::on_createButton_clicked()
{
    QString title = QInputDialog::getText(this, "Create Problem", "Enter Problem Title:");
    if (title.isEmpty()) return;

    QString description = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Problem Description:");
    QString inputFormat = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Input Format:");
    QString outputFormat = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Output Format:");
    QString constraints = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Constraints:");
    QString example = QInputDialog::getMultiLineText(this, "Create Problem", "Enter Example:");

    Database db;
    if (db.connect() && db.createProblem(title, description, inputFormat, outputFormat, constraints, example, {})) // Empty topic IDs for now
    {
        QMessageBox::information(this, "Success", "Problem created successfully!");
        populateTable();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to create problem.");
    }
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

    Database db;
    if (db.connect() && db.deleteProblem(id)) {
        QMessageBox::information(this, "Success", "Problem deleted successfully!");
        populateTable();
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete problem.");
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

    int id = ui->tableWidget->item(row, 0)->text().toInt();
    QString title = QInputDialog::getText(this, "Edit Problem", "Enter new title:", QLineEdit::Normal, ui->tableWidget->item(row, 1)->text());
    QString description = QInputDialog::getText(this, "Edit Problem", "Enter new description:", QLineEdit::Normal, ui->tableWidget->item(row, 2)->text());
    QString inputFormat = QInputDialog::getText(this, "Edit Problem", "Enter new input format:", QLineEdit::Normal, ui->tableWidget->item(row, 3)->text());
    QString outputFormat = QInputDialog::getText(this, "Edit Problem", "Enter new output format:", QLineEdit::Normal, ui->tableWidget->item(row, 4)->text());
    QString constraints = QInputDialog::getText(this, "Edit Problem", "Enter new constraints:", QLineEdit::Normal, ui->tableWidget->item(row, 5)->text());
    QString example = QInputDialog::getText(this, "Edit Problem", "Enter new example:", QLineEdit::Normal, ui->tableWidget->item(row, 6)->text());

    Database db;
    if (db.connect() && db.updateProblem(id, title, description, inputFormat, outputFormat, constraints, example))
    {
        QMessageBox::information(this, "Success", "Problem updated successfully!");
        populateTable();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to update problem.");
    }
}

void problemManager::on_viewButton_clicked()
{
    populateTable();
}
