#include "UserDB.h"
#include <functional>

std::string UserDB::hash_password(const std::string &password) {
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
    if (!execute_SQL(sql)) {
        return false;
    }
    return true;
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
        return false;
    }

    bool result = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return result;
}

std::string UserDB::getUserRole(const std::string &username) {
    const char *sql = "SELECT role FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;
    std::string role = "";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            role = (const char *)sqlite3_column_text(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    return role;
}
