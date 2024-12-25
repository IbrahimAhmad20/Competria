#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QDialog>

namespace Ui
{
class admindashboard;
}

class admindashboard : public QDialog
{
    Q_OBJECT

public:
    explicit admindashboard(QWidget *parent = nullptr);
    ~admindashboard();

private slots:
void on_problemManagerButton_clicked();

    void on_userManagerButton_clicked();


private:
    Ui::admindashboard *ui;
};

#endif
