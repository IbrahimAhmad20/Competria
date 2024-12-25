
#ifndef CREATEUSER_H
#define CREATEUSER_H

#include <QDialog>

namespace Ui
{
class createuser;
}

class createuser : public QDialog
{
    Q_OBJECT

public:
    explicit createuser(QWidget *parent = nullptr);
    ~createuser();

private slots:
    void on_create_user_button_clicked();

private:
    Ui::createuser *ui;
};

#endif
