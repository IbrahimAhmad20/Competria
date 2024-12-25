#ifndef SUBMISSIONMANAGER_H
#define SUBMISSIONMANAGER_H

#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class SubmissionManager {
public:
    SubmissionManager(Database &database);
    int saveSubmission(int userId, int problemId, const QString &code);
    int getLastSubmissionId() const;

private:
    Database &db;
    int lastSubmissionId;
};

#endif // SUBMISSIONMANAGER_H
