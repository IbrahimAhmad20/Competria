#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QDebug>

class Database
{
public:
    Database();
    ~Database();

    bool connect();

    bool createUser(const QString& email, const QString& username, const QString& password);
    bool validateUser(const QString& username, const QString& password);
    bool validateAdmin(const QString& adminUsername, const QString& adminPassword);
    bool deleteUser(int id);
    QSqlQuery getAllUsers();
    bool updateUser(int id, const QString& email, const QString& username, const QString& password);


    bool createProblem(const QString& title, const QString& description,
                       const QString& inputFormat, const QString& outputFormat,
                       const QString& constraints, const QString& example,
                       const QString& topic);
    QSqlQuery getAllProblems();
    bool updateProblem(int id, const QString &title, const QString &description,
                                 const QString &inputFormat, const QString &outputFormat,
                                 const QString &constraints, const QString &example,
                                 const QString &topic);
    bool deleteProblem(int id);
    QSqlDatabase getDatabase() const;
    QString hashPassword(const QString& password);
    bool submitSolution(int problemId, int userId, const QString &submission);
    bool saveSubmission(int userId, int problemId, const QString &code);
    bool saveResult(int submissionId, const QString &status, double runtime, double memoryUsed);
    void setCurrentUserId(int userId)
    {
        currentUserId = userId;
    }

    int getCurrentUserId() const {
        return currentUserId;
    }
private:
    QSqlDatabase db;
      int currentUserId = -1; // Default: no user logged in
};


#endif
