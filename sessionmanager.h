#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>

class sessionManager
{
public:
    // Singleton instance
    static sessionManager& instance();

    // Setters
    void setLoggedInUser(int userId, const QString& username);
    void logout();

    // Getters
    bool isLoggedIn() const;
    int getUserId() const;
    QString getUsername() const;

private:
    // Private constructor for singleton
    sessionManager();
    ~sessionManager();

    // Disable copy and assignment
    sessionManager(const sessionManager&) = delete;
    sessionManager& operator=(const sessionManager&) = delete;

    bool loggedIn;
    int userId;
    QString username;
};

#endif // SESSIONMANAGER_H
