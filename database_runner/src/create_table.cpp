#include "database.h"

int main() {
    Database db("results.db");
    std::string sql = R"(
        CREATE TABLE IF NOT EXISTS solutions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            code TEXT NOT NULL,
            pass_compile INTEGER NOT NULL,
            compile_error TEXT,
            total_tests INTEGER NOT NULL,
            passed_tests INTEGER NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";
    if (db.execute_SQL(sql)) {
        std::cout << "Таблица создана!" << std::endl;
    } else {
        std::cout << "Ошибка при создании таблицы" << std::endl;
    }
}