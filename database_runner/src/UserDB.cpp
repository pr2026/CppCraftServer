#include "UserDB.h"
#include <functional>

std::string hash_password(const std::string &password) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}

std::optional<int> UserDB::get_user_id(const std::string &username) {
    std::string sql =
        "SELECT id FROM users WHERE username = '" + username + "';";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    std::optional<int> result = std::nullopt;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return result;
}

bool UserDB::userExists(const std::string &username) {
    auto id = get_user_id(username);
    return id.has_value();
}

bool UserDB::addUser(
    const std::string &username,
    const std::string &password,
    const std::string &role
) {
    if (userExists(username)) {
        return false;
    }
    std::string password_h = hash_password(password);

    std::string sql = "INSERT INTO users (username, password, role) VALUES ('" +
                      username + "', '" + password_h + "', '" + role + "');";

    return execute_SQL(sql);
}

bool UserDB::checkPassword(
    const std::string &username,
    const std::string &password
) {
    std::string password_h = hash_password(password);

    std::string sql = "SELECT id FROM users WHERE username = '" + username +
                      "' AND password = '" + password_h + "';";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    bool result = false;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = true;
    }

    sqlite3_finalize(stmt);
    return result;
}