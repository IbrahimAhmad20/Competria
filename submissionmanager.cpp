#include "submissionmanager.h"

SubmissionManager::SubmissionManager(Database &database)
    : db(database), lastSubmissionId(-1) {}

int SubmissionManager::saveSubmission(int userId, int problemId, const QString &code) {
    QSqlQuery query(db.getDatabase());
    query.prepare("INSERT INTO Submissions (Users_Id, Problem_Id, Submission_Time, Code, Language) "
                  "VALUES (:userId, :problemId, :submissionTime, :code, :language)");
    query.bindValue(":userId", userId);
    query.bindValue(":problemId", problemId);
    query.bindValue(":submissionTime", QDateTime::currentDateTime());
    query.bindValue(":code", code);
    query.bindValue(":language", "C++");

    // Execute the INSERT query
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return -1;
    }

    // Retrieve the last inserted ID using @@IDENTITY
    QSqlQuery identityQuery(db.getDatabase());
    if (!identityQuery.exec("SELECT @@IDENTITY")) {
        qDebug() << "Failed to retrieve last inserted ID:" << identityQuery.lastError().text();
        return -1;
    }

    if (identityQuery.next()) {
        int lastSubmissionId = identityQuery.value(0).toInt();
        qDebug() << "Submission saved successfully with ID:" << lastSubmissionId;
        return lastSubmissionId;
    }

    qDebug() << "No last inserted ID retrieved.";
    return -1;
}


int SubmissionManager::getLastSubmissionId() const
{
    return lastSubmissionId;
}
