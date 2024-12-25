#include "sessionmanager.h"

sessionManager& sessionManager::instance()
{
    static sessionManager instance;
    return instance;
}

// Constructor
sessionManager::sessionManager() : loggedIn(false), userId(-1), username("") {}

// Destructor
sessionManager::~sessionManager() {}

// Set the logged-in user
void sessionManager::setLoggedInUser(int userId, const QString& username) {
    this->userId = userId;
    this->username = username;
    this->loggedIn = true;
}

// Logout and clear the session
void sessionManager::logout()
{
    this->userId = -1;
    this->username.clear();
    this->loggedIn = false;
}

bool sessionManager::isLoggedIn() const
{
    return loggedIn;
}

int sessionManager::getUserId() const
{
    return userId;
}

QString sessionManager::getUsername() const
{
    return username;
}
