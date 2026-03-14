#pragma once
#include <string>

class Storage{
public:
    virtual ~Storage() = default;
    virtual bool userExists(const std::string& username) = 0;
    virtual bool addUser(const std::string& username, const std::string& password) = 0;
    virtual bool checkPassword(const std::string& username, const std::string& password) = 0;
};