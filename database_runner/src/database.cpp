#include "Database.h"
#include <functional>
#include <iostream>

Database::Database(const std::string &filename) {
    int retunt_code = sqlite3_open(filename.c_str(), &db);
    if (retunt_code != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

bool Database::execute_SQL(const std::string &sql) {
    if (!db) {
        return false;
    }

    char *err_msg = nullptr;
    int return_code = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);

    if (return_code != SQLITE_OK) {
        std::cerr << "Ошибка SQL: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}

long long Database::last_insert_row_id() const {
    return sqlite3_last_insert_rowid(db);
}