#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include "database.h"

namespace Ui
{
class usermanager;
}

class usermanager : public QDialog
{
    Q_OBJECT

public:
    explicit usermanager(QWidget *parent = nullptr);
    ~usermanager();

private slots:
    void loadUsers();

    void on_adduserButton_clicked();

    void on_deleteuserButton_clicked();

    void on_edituserButton_clicked();

private:
    Ui::usermanager *ui;
    Database db;
    void setupConnections();
};

#endif
