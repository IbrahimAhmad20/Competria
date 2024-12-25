#include "submissionhandler.h"
#include <QSqlDatabase>
#include <QSqlQuery>

SubmissionHandler::SubmissionHandler() {
    db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
    }
}

SubmissionHandler::~SubmissionHandler() {}

bool SubmissionHandler::addSubmission(int userId, int problemId, const QString& code, const QString& submissionTime) {
    QSqlQuery query;
    query.prepare("INSERT INTO Submissions (Users_Id, Problem_Id, Code, Submission_Time) VALUES (?, ?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(problemId);
    query.addBindValue(code);
    query.addBindValue(submissionTime);

    if (!query.exec()) {
        qDebug() << "Failed to add submission:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQuery SubmissionHandler::getAllSubmissions() {
    QSqlQuery query;
    query.exec("SELECT * FROM Submissions");
    return query;
}

QSqlQuery SubmissionHandler::getSubmissionsByUser(int userId) {
    QSqlQuery query;
    query.prepare("SELECT * FROM Submissions WHERE Users_Id = ?");
    query.addBindValue(userId);
    query.exec();
    return query;
}

QSqlQuery SubmissionHandler::getSubmissionsByProblem(int problemId) {
    QSqlQuery query;
    query.prepare("SELECT * FROM Submissions WHERE Problem_Id = ?");
    query.addBindValue(problemId);
    query.exec();
    return query;
}

bool SubmissionHandler::deleteSubmission(int submissionId) {
    QSqlQuery query;
    query.prepare("DELETE FROM Submissions WHERE ID = ?");
    query.addBindValue(submissionId);

    if (!query.exec()) {
        qDebug() << "Failed to delete submission:" << query.lastError().text();
        return false;
    }
    return true;
}
