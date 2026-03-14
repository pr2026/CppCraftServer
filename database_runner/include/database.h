#pragma once 

#include <string>
#include <sqlite3.h>
#include <iostream>

class Database {
    sqlite3* db;   
    public:
    Database(const std::string& filename);
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    bool execute_SQL(const std::string& sql);
};