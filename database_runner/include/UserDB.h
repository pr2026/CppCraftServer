#pragma once
#include <optional>
#include "Database.h"
#include "storage/Storage.h"

class UserDB : public Database, public Storage {
    std::string hash_password(const std::string &password);
    std::optional<int> get_user_id(const std::string &username);

public:
    using Database::Database;

    bool userExists(const std::string &username) override;

    // need change just for this night
    bool addUser(const std::string &username, const std::string &password)
        override;
    //
    int addUser(
        const std::string &username,
        const std::string &password,
        const std::string &role = "student"
    );
    bool checkPassword(const std::string &username, const std::string &password)
        override;
};