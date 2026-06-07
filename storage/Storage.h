#pragma once
#include <string>

struct User {
    int id;
    std::string username;
    std::string password;
    std::string role;
};

class Storage {
public:
    virtual ~Storage() = default;
    virtual bool userExists(const std::string &username) = 0;
    virtual bool
    checkPassword(const std::string &username, const std::string &password) = 0;
    virtual bool addUser(const std::string& username, const std::string& password, const std::string& role) = 0;
    virtual std::string getUserRole(const std::string& username) = 0;
};