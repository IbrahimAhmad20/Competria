#include "database.h"
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>
#include "sessionmanager.h"

Database::Database()
{
    db = QSqlDatabase::addDatabase("QODBC");
}

Database::~Database()
{
    if (db.isOpen())
    {
        db.close();
    }

}


bool Database::connect()
{

    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
        if (db.isOpen()) {
            return true;
        }
    } else
    {
        db = QSqlDatabase::addDatabase("QODBC", "qt_sql_default_connection");
    }

    QString connectionString = QString(
        "DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};"
        "DBQ=C:\\Users\\muhammed\\Documents\\Database1.accdb;"
        );

    db.setDatabaseName(connectionString);

    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully.";
    return true;
}
QSqlDatabase Database::getDatabase() const
{
    return db;
}
QString Database::hashPassword(const QString &password)
{
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool Database::createUser(const QString &email, const QString &username, const QString &password)
{
    if (!db.isOpen()) return false;

    QString Password = hashPassword(password);

    QSqlQuery query(db);
    query.prepare("INSERT INTO Users (Email, Username, Password) VALUES (?, ?, ?)");
    query.addBindValue(email);
    query.addBindValue(username);
    query.addBindValue(Password);

    if (!query.exec())
    {
        qDebug() << "Insert user failed:" << query.lastError().text();
        return false;
    }

    return true;
}

/*bool Database::validateUser(const QString &username, const QString &password)
{
    if (!db.isOpen()) return false;

    QString hashedPassword = hashPassword(password);

    QSqlQuery query(db);
    query.prepare("SELECT * FROM Users WHERE Username = ? AND Password = ?");
    query.addBindValue(username);
    query.addBindValue(hashedPassword);

    if (!query.exec())
    {
        qDebug() << "Validation query failed:" << query.lastError().text();
        return false;
    }

    return query.next();
}
*/
bool Database::validateUser(const QString &username, const QString &password) {
    if (!db.isOpen()) return false;

    QString hashedPassword = hashPassword(password);

    QSqlQuery query(db);
    query.prepare("SELECT ID FROM Users WHERE Username = ? AND Password = ?");
    query.addBindValue(username);
    query.addBindValue(hashedPassword);

    if (!query.exec()) {
        qDebug() << "Validation query failed:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int userId = query.value("ID").toInt();
        sessionManager::instance().setLoggedInUser(userId, username); // Set session data
        return true;
    }

    return false;
}

bool Database::validateAdmin(const QString &adminUsername, const QString &adminPassword)
{
    if (!db.isOpen()) return false;

    QString hashedPassword = hashPassword(adminPassword);

    QSqlQuery query(db);
    query.prepare("SELECT * FROM Admins WHERE Username = ? AND Password = ?");
    query.addBindValue(adminUsername);
    query.addBindValue(hashedPassword);

    if (!query.exec())





    {
        qDebug() << "Admin validation query failed:" << query.lastError().text();
        return false;
    }

    return query.next();
}

bool Database::createProblem(const QString &title, const QString &description,
                             const QString &inputFormat, const QString &outputFormat,
                             const QString &constraints, const QString &example,
                             const QString &topic)
{
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("INSERT INTO Problems (Title, Description, Input_Format, Output_Format, Constraints, Example, Topic) "
                  "VALUES (:title, :description, :inputFormat, :outputFormat, :constraints, :example, :topic)");
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":inputFormat", inputFormat);
    query.bindValue(":outputFormat", outputFormat);
    query.bindValue(":constraints", constraints);
    query.bindValue(":example", example);
    query.bindValue(":topic", topic);

    if (!query.exec()) {
        qDebug() << "Insert problem failed:" << query.lastError().text();
        return false;
    }
    qDebug() << "Creating problem with:" << title << description << inputFormat << outputFormat << constraints << example << topic;

    return true;
}


QSqlQuery Database::getAllProblems()
{
    if (!db.isOpen()) {
        qDebug() << "Database is not open.";
        return QSqlQuery();
    }

    QSqlQuery query(db);
    query.prepare("SELECT ID, Title, Description, Input_Format, Output_Format, Constraints, Example FROM Problems");

    if (!query.exec()) {
        qDebug() << "Failed to fetch problems:" << query.lastError().text();
        return QSqlQuery();
    }

    return query;
}




bool Database::updateProblem(int id, const QString &title, const QString &description,
                             const QString &inputFormat, const QString &outputFormat,
                             const QString &constraints, const QString &example,
                             const QString &topic)
{
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE Problems SET Title = ?, Description = ?, Input_Format = ?, "
                  "Output_Format = ?, Constraints = ?, Example = ?, Topic = ? WHERE ID = ?");
    query.addBindValue(title);
    query.addBindValue(description);
    query.addBindValue(inputFormat);
    query.addBindValue(outputFormat);
    query.addBindValue(constraints);
    query.addBindValue(example);
    query.addBindValue(topic);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "Update problem failed:" << query.lastError().text();
        return false;
    }

    return true;
}


bool Database::deleteProblem(int id)
{
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("DELETE FROM Problems WHERE ID = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        qDebug() << "Delete problem failed:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQuery Database::getAllUsers()
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM Users");

    if (!query.exec())
    {
        qDebug() << "Failed to fetch users:" << query.lastError().text();
    }

    return query;
}
bool Database::deleteUser(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM Users WHERE ID = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        qDebug() << "Delete user failed:" << query.lastError().text();
        return false;
    }

    return true;
}
bool Database::updateUser(int id, const QString &email, const QString &username, const QString &password) {
    QSqlQuery query(db);
    query.prepare("UPDATE Users SET Email = ?, Username = ?, Password = ? WHERE ID = ?");
    query.addBindValue(email);
    query.addBindValue(username);
    query.addBindValue(password);
    query.addBindValue(id);

    if (!query.exec())
    {
        qDebug() << "Update user failed:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::saveSubmission(int userId, int problemId, const QString &code)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO Submissions (Users_Id, Problem_Id, Submission_Time, Code) "
                  "VALUES (:userId, :problemId, :submissionTime, :code)");
    query.bindValue(":userId", userId);
    query.bindValue(":problemId", problemId);
    query.bindValue(":submissionTime", QDateTime::currentDateTime());
    query.bindValue(":code", code);

    return query.exec();
}
bool Database::saveResult(int submissionId, const QString &status, double runtime, double memoryUsed)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO Results (Submission_Id, Status, Runtime, Memory_Used) "
                  "VALUES (:submissionId, :status, :runtime, :memoryUsed)");
    query.bindValue(":submissionId", submissionId);
    query.bindValue(":status", status);
    query.bindValue(":runtime", runtime);
    query.bindValue(":memoryUsed", memoryUsed);

    return query.exec();
}
