#include "database.h"
#include <iostream>

Database::Database(const std::string& filename) {
    int retunt_code = sqlite3_open(filename.c_str(), &db); //открываем бд
    if (retunt_code != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        std::cout << "База данных открыта: " << filename << std::endl;
    }
}
Database::~Database() {
    if (db) {
        sqlite3_close(db);
        std::cout << "База данных закрыта" << std::endl;
    }
}
bool Database::execute_SQL(const std::string& sql) {
    if (!db) {
        std::cerr << "Нет соединения с БД" << std::endl;
        return false;
    }
    
    char* errMsg = nullptr; 
    int return_code = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    
    if (return_code != SQLITE_OK) {
        std::cerr << "Ошибка SQL: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    std::cout << "SQL выполнен успешно: " << sql << std::endl;
    return true;
}
