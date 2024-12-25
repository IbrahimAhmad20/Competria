#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "database.h"
#include "solveproblemwindow.h"
#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class userdashboard;
}

class userdashboard : public QDialog
{
    Q_OBJECT

public:
    explicit userdashboard(QWidget *parent = nullptr);
    ~userdashboard();

private slots:
    void loadProblems();
    void onProblemSelected(int row, int column);

    void on_SolveChallenge_clicked();

private:
    Ui::userdashboard *ui;
    Database db;
    QStandardItemModel *model;
};

#endif // USERDASHBOARD_H
