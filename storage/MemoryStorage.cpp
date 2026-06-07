#include "MemoryStorage.h"

std::unordered_map<std::string, User> MemoryStorage::users;
std::mutex MemoryStorage::m;

bool MemoryStorage::userExists(const std::string& username) {
    std::lock_guard<std::mutex> lock(m);
    return users.find(username) != users.end();
}

bool MemoryStorage::addUser(const std::string& username, const std::string& password, const std::string& role) {
    std::lock_guard<std::mutex> lock(m);
    if (users.find(username) != users.end()) return false;
    User u;
    u.id = users.size() + 1;
    u.username = username;
    u.password = password;
    u.role = role;
    users[username] = u;
    return true;
}

bool MemoryStorage::checkPassword(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(m);
    auto it = users.find(username);
    if (it == users.end()) return false;
    return it->second.password == password;
}

std::string MemoryStorage::getUserRole(const std::string& username) {
    std::lock_guard<std::mutex> lock(m);
    auto it = users.find(username);
    if (it == users.end()) return "student";
    return it->second.role;
}

std::optional<User> MemoryStorage::getUserByUsername(const std::string& username) {
    std::lock_guard<std::mutex> lock(m);
    auto it = users.find(username);
    if (it == users.end()) return std::nullopt;
    return it->second;
}