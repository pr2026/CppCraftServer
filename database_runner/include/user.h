#pragma once
#include <string>

struct User {
    int id;
    std::string username;
    std::string password;
    std::string role;
    User() : id(0) {}
    User(int id, const std::string& username, const std::string& role): id(id), username(username), role(role),  {}
};

