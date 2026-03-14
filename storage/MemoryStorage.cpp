 #include "storage/MemoryStorage.h"

std::unordered_map<std::string, std::string> MemoryStorage::users;
std::mutex MemoryStorage::m;

bool MemoryStorage::userExists(const std::string& username){
    std::lock_guard<std::mutex> lock(m);
    return users.find(username) != users.end();
}

bool MemoryStorage::addUser(const std::string& username, const std::string& password){
    std::lock_guard<std::mutex> lock(m);
    if (users.find(username) != users.end()){
        return false;
    }
    users[username] = password;
    return true;
}

bool MemoryStorage::checkPassword(const std::string& username, const std::string& password){
    std::lock_guard<std::mutex> lock(m);
    auto x = users.find(username);
    if (x == users.end()) return false;
    return x -> second == password;
}