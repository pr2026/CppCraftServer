#pragma once
#include "storage/Storage.h"
#include <unordered_map>
#include <mutex>

class MemoryStorage : public Storage{
public:
    bool userExists(const std::string& username) override;
    bool addUser(const std::string& username, const std::string& password) override;
    bool checkPassword(const std::string& username, const std::string& password) override;

private:
    static std::unordered_map<std::string, std::string> users;
    static std::mutex m;
};