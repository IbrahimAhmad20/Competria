#ifndef PROBLEMMANAGER_H
#define PROBLEMMANAGER_H

#include <QDialog>

namespace Ui
{
class problemManager;
}

class problemManager : public QDialog
{
    Q_OBJECT

public:
    explicit problemManager(QWidget *parent = nullptr);
    ~problemManager();

private slots:
    void on_createButton_clicked();
    void on_viewButton_clicked();
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void populateTable();

private:
    Ui::problemManager *ui;
};

#endif // PROBLEMMANAGER_H
