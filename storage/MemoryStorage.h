#pragma once
#include <mutex>
#include <unordered_map>
#include "storage/Storage.h"

class MemoryStorage : public Storage {
public:
    bool userExists(const std::string &username) override;
    bool addUser(const std::string &username, const std::string &password)
        override;
    bool checkPassword(const std::string &username, const std::string &password)
        override;

private:
    static std::unordered_map<std::string, std::string> users;
    static std::mutex m;
};