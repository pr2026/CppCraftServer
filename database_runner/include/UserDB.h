#pragma once
#include <optional>
#include "Database.h"
#include "storage/Storage.h"

class UserDB : public Database, public Storage {
    std::string hash_password(const std::string &password);
    

public:
    using Database::Database;

    std::optional<int> get_user_id(const std::string &username);

    bool userExists(const std::string &username) override;

    bool addUser(
        const std::string &username,
        const std::string &password,
        const std::string &role = "student"
    ) override;
    std::string getUserRole(const std::string& username) override;

    bool checkPassword(const std::string &username, const std::string &password)
        override;
};
