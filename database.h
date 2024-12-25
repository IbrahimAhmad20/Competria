#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

class Database
{
public:
    Database();
   ~Database();

    bool connect();
    bool createUser(const QString& email, const QString& username, const QString& password);
    bool validateUser(const QString& username, const QString& password);
    bool validateAdmin(const QString& adminUsername, const QString& adminPassword);
   // bool createProblem(const QString& title, const QString& description);
    bool createProblem(QString title, QString description, QString inputFormat, QString outputFormat, QString constraints, QString example, QList<int> topicIDs) ;
    QSqlQuery getAllProblems();
   // bool updateProblem(int id, const QString& title, const QString& description);
    bool updateProblem(int id, const QString &title, const QString &description, const QString &inputFormat,
                                 const QString &outputFormat, const QString &constraints, const QString &example);
    bool deleteProblem(int id);
    QSqlQuery getAllUsers();
    bool updateUser(int id, const QString &email, const QString &username, const QString &password);
    int getTopicIDByName(const QString &topicName);

    bool deleteUser(int id) ;
    QString hashPassword(const QString &password);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
