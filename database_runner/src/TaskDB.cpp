#include "TaskDB.h"

int TaskDB::add_task(
    const std::string &title,
    const std::string &description,
    const std::string &difficulty
) {
    std::string sql =
        "INSERT INTO tasks (title, description, difficulty) VALUES ('" + title +
        "', '" + description + "', '" + difficulty + "');";
    if (!execute_SQL(sql)) {  // need to change on smt usefull
        return -1;
    }
    return sqlite3_last_insert_rowid(db);
}

std::vector<Task> TaskDB::get_all_tasks() {
    std::vector<Task> tasks;
    std::string sql = "SELECT id, title, description, difficulty FROM tasks;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return tasks;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {  // read new row
        Task task;
        task.id = sqlite3_column_int(stmt, 0);
        task.title = (const char *)sqlite3_column_text(stmt, 1);
        task.description = (const char *)sqlite3_column_text(stmt, 2);
        task.difficulty = (const char *)sqlite3_column_text(stmt, 3);
        tasks.push_back(task);
    }
    sqlite3_finalize(stmt);
    return tasks;
}

std::optional<Task> TaskDB::get_task_by_id(int id) {
    std::optional<Task> result = std::nullopt;

    std::string sql =
        "SELECT id, title, description, difficulty FROM tasks WHERE id = " +
        std::to_string(id) + ";";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        Task task;
        task.id = sqlite3_column_int(stmt, 0);
        task.title = (const char *)sqlite3_column_text(stmt, 1);
        task.description = (const char *)sqlite3_column_text(stmt, 2);
        task.difficulty = (const char *)sqlite3_column_text(stmt, 3);
        result = task;
    }
    sqlite3_finalize(stmt);
    return result;
}

bool TaskDB::updateTask(int id, const Task &task) {
    std::string sql = "UPDATE tasks SET title = '" + task.title +
                      "', description = '" + task.description +
                      "', difficulty = '" + task.difficulty +
                      "' WHERE id = " + std::to_string(id) + ";";
    return execute_SQL(sql);
}

bool TaskDB::deleteTask(int id) {
    std::string sql =
        "DELETE FROM tasks WHERE id = " + std::to_string(id) + ";";
    return db.execute_SQL(sql);
}

bool TaskDB::add_test(
    int task_id,
    const std::string &input,
    const std::string &expected
) {
    std::string sql =
        "INSERT INTO tests (task_id, input, expected_output) VALUES (" +
        std::to_string(task_id) + ", '" + input + "', '" + expected + "');";

    return execute_SQL(sql);
}

std::vector<Test> TaskDB::get_tests_for_task(int task_id) {
    std::vector<Test> tests;

    std::string sql =
        "SELECT id, input, expected_output FROM tests WHERE task_id = " +
        std::to_string(task_id) + ";";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return tests;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Test test;
        test.id = sqlite3_column_int(stmt, 0);
        test.task_id = task_id;
        test.input = (const char *)sqlite3_column_text(stmt, 1);
        test.expected_output = (const char *)sqlite3_column_text(stmt, 2);
        tests.push_back(test);
    }

    sqlite3_finalize(stmt);
    return tests;
}
