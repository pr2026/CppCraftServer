#pragma once
#include <mutex>
#include <unordered_map>
#include "storage/Storage.h"

class MemoryStorage : public Storage {
public:
    bool userExists(const std::string &username) override;
    bool checkPassword(const std::string &username, const std::string &password)
        override;
    bool addUser(const std::string& username, const std::string& password, const std::string& role) override;
    std::string getUserRole(const std::string& username) override;
    

private:
    static std::unordered_map<std::string, User> users;
    static std::mutex m;
    //int nextId = 1;
};