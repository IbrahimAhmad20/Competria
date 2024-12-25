#ifndef SOLVEPROBLEMWINDOW_H
#define SOLVEPROBLEMWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "database.h"
#include "qdialog.h"
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>

namespace Ui {
class solveproblemwindow;
}

class solveproblemwindow : public QDialog

{
    Q_OBJECT

public:
    solveproblemwindow(Database &db, int problemId, QWidget *parent = nullptr);

    ~solveproblemwindow();

private slots:
    void runCode();
 void on_submitButton_clicked();
    void saveResults(int submissionId) ;
private:
    Ui::solveproblemwindow *ui;
    int problemId;
     Database &db;
    QProcess *process;
      QsciScintilla *scintilla;
};

#endif // SOLVEPROBLEMWINDOW_H
