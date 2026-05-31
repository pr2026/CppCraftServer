#pragma once

#include <sqlite3.h>
#include <string>

class Database {
protected:
    sqlite3 *db;

public:
    Database(const std::string &filename);
    virtual ~Database();
    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;
    bool execute_SQL(const std::string &sql);
    long long last_insert_row_id() const;
};