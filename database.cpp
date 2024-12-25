#include "database.h"
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>

Database::Database()
{
    db = QSqlDatabase::addDatabase("QODBC");
}

Database::~Database()
{
    if (db.isOpen())
    {
        db.close(); // Close the database connection on destruction
    }
}

/*bool Database::connect()
{
    QString connectionString = QString(
        "DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};"
        "DBQ=C:\\Users\\muhammed\\Documents\\Database1.accdb;"
        );

    db.setDatabaseName(connectionString);

    if (!db.open())
    {
        qDebug() << "Database connection failed:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully.";
    return true;
}
*/
bool Database::connect()
{
    // Check for existing connection
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
        if (db.isOpen()) {
            return true; // Use existing open connection
        }
    } else {
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
QString Database::hashPassword(const QString &password)
{
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}
/*bool Database::createUser(const QString &email, const QString &username, const QString &password) {
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("INSERT INTO Users (Email, Username, Password) VALUES (?, ?, ?)");
    query.addBindValue(email);
    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) {
        qDebug() << "Insert user failed:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::validateUser(const QString &username, const QString &password) {
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM Users WHERE Username = ? AND Password = ?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) {
        qDebug() << "Validation query failed:" << query.lastError().text();
        return false;
    }

    return query.next(); // Returns true if a matching record is found
}

bool Database::validateAdmin(const QString &adminUsername, const QString &adminPassword) {
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM Admins WHERE Username = ? AND Password = ?");
    query.addBindValue(adminUsername);
    query.addBindValue(adminPassword);

    if (!query.exec()) {
        qDebug() << "Admin validation query failed:" << query.lastError().text();
        return false;
    }

    return query.next(); // Returns true if a matching admin record is found
}
*/
bool Database::createUser(const QString &email, const QString &username, const QString &password)
{
    if (!db.isOpen()) return false;

    QString hashedPassword = hashPassword(password);

    QSqlQuery query(db);
    query.prepare("INSERT INTO Users (Email, Username, Password) VALUES (?, ?, ?)");
    query.addBindValue(email);
    query.addBindValue(username);
    query.addBindValue(hashedPassword);

    if (!query.exec()) {
        qDebug() << "Insert user failed:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::validateUser(const QString &username, const QString &password)
{
    if (!db.isOpen()) return false;

    QString hashedPassword = hashPassword(password);

    QSqlQuery query(db);
    query.prepare("SELECT * FROM Users WHERE Username = ? AND Password = ?");
    query.addBindValue(username);
    query.addBindValue(hashedPassword);

    if (!query.exec()) {
        qDebug() << "Validation query failed:" << query.lastError().text();
        return false;
    }

    return query.next(); // Returns true if a matching record is found
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

    return query.next(); // Returns true if a matching admin record is found
}
/*bool Database::createProblem(const QString &title, const QString &description) {
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("INSERT INTO Problems (Title, Description) VALUES (?, ?)");
    query.addBindValue(title);
    query.addBindValue(description);

    if (!query.exec()) {
        qDebug() << "Create problem failed:" << query.lastError().text();
        return false;
    }

    return true;
} */
bool Database::createProblem(QString title, QString description, QString inputFormat, QString outputFormat, QString constraints, QString example, QList<int> topicIDs)
{
    if (!db.isOpen()) {
        qDebug() << "Database is not open.";
        return false;
    }

    QSqlQuery query(db);

    // Step 1: Insert the problem into the Problems table
    query.prepare("INSERT INTO Problems (Title, Description, Input_Format, Output_Format, Constraints, Example) "
                  "VALUES (:title, :description, :inputFormat, :outputFormat, :constraints, :example)");
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":inputFormat", inputFormat);
    query.bindValue(":outputFormat", outputFormat);
    query.bindValue(":constraints", constraints);
    query.bindValue(":example", example);

    if (!query.exec()) {
        qDebug() << "Failed to insert problem:" << query.lastError().text();
        return false;
    }

    int problemID = query.lastInsertId().toInt(); // Retrieve the newly inserted Problem ID

    // Step 2: Link the problem to multiple topic IDs
    for (int topicID : topicIDs) {
        QSqlQuery linkQuery(db);
        linkQuery.prepare("INSERT INTO ProblemTopics (Problem_ID, Topic_ID) VALUES (:problemID, :topicID)");
        linkQuery.bindValue(":problemID", problemID);
        linkQuery.bindValue(":topicID", topicID);

        if (!linkQuery.exec()) {
            qDebug() << "Failed to link problem with topic ID:" << topicID << "-" << linkQuery.lastError().text();
            return false; // Handle failure (optional: continue instead of returning)
        }
    }

    qDebug() << "Problem created successfully and linked to topics.";
    return true;
}

int Database::getTopicIDByName(const QString &topicName)
{
    if (!db.isOpen()) {
        qDebug() << "Database is not open.";
        return -1;
    }

    QSqlQuery query(db);
    query.prepare("SELECT ID FROM Topics WHERE Topic_Name = :name");
    query.bindValue(":name", topicName);

    if (query.exec() && query.next()) {
        return query.value(0).toInt(); // Return the ID if found
    } else {
        qDebug() << "Topic not found:" << topicName << query.lastError().text();
        return -1; // Return -1 if not found
    }
}

/*QSqlQuery Database::getAllProblems()
{
    if (!db.isOpen())
    {
        qDebug() << "Database is not open.";
        return QSqlQuery(); // Return an empty query
    }

    QSqlQuery query(db);
    query.prepare("SELECT * FROM Problems");

    if (!query.exec())
    {
        qDebug() << "Failed to fetch problems:" << query.lastError().text();
    }

    return query;
}*/
/*QSqlQuery Database::getAllProblems()
{
    if (!db.isOpen())
    {
        qDebug() << "Database is not open.";
        return QSqlQuery(); // Return an empty query
    }

    QSqlQuery query(db);
    query.prepare("SELECT Problems.*, GROUP_CONCAT(Topics.Topic_Name) AS Topics "
                  "FROM Problems "
                  "LEFT JOIN ProblemTopics ON Problems.ID = ProblemTopics.Problem_ID "
                  "LEFT JOIN Topics ON ProblemTopics.Topic_ID = Topics.ID "
                  "GROUP BY Problems.ID, Problems.Title, Problems.Description, Problems.Input_Format, "
                  "Problems.Output_Format, Problems.Constraints, Problems.Example");

    if (!query.exec())
    {
        qDebug() << "Failed to fetch problems:" << query.lastError().text();
    }

    return query;
}*/
QSqlQuery Database::getAllProblems()
{
    if (!db.isOpen()) {
        qDebug() << "Database is not open.";
        return QSqlQuery(); // Return an empty query
    }

    QSqlQuery query(db);
    query.prepare("SELECT ID, Title, Description, Input_Format, Output_Format, Constraints, Example FROM Problems");

    if (!query.exec()) {
        qDebug() << "Failed to fetch problems:" << query.lastError().text();
        return QSqlQuery(); // Return an empty query if execution fails
    }

    return query; // Return the executed query
}



/*bool Database::updateProblem(int id, const QString &title, const QString &description)
{
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE Problems SET Title = ?, Description = ? WHERE ID = ?");
    query.addBindValue(title);
    query.addBindValue(description);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "Update problem failed:" << query.lastError().text();
        return false;
    }

    return true;
}*/
bool Database::updateProblem(int id, const QString &title, const QString &description, const QString &inputFormat,
                             const QString &outputFormat, const QString &constraints, const QString &example) {
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE Problems SET Title = ?, Description = ?, Input_Format = ?, Output_Format = ?, "
                  "Constraints = ?, Example = ? WHERE ID = ?");
    query.addBindValue(title);
    query.addBindValue(description);
    query.addBindValue(inputFormat);
    query.addBindValue(outputFormat);
    query.addBindValue(constraints);
    query.addBindValue(example);
    query.addBindValue(id);

    if (!query.exec())
    {
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

