#ifndef SUBMISSIONHANDLER_H
#define SUBMISSIONHANDLER_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class SubmissionHandler
{
public:
    SubmissionHandler();
    ~SubmissionHandler();

    bool addSubmission(int userId, int problemId, const QString& code, const QString& submissionTime);
    QSqlQuery getAllSubmissions();
    QSqlQuery getSubmissionsByUser(int userId);
    QSqlQuery getSubmissionsByProblem(int problemId);
    bool deleteSubmission(int submissionId);

private:
    QSqlDatabase db;
};

#endif
