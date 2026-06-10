#include "TaskDB.h"

int TaskDB::addTask(
    const std::string &title,
    const std::string &description,
    int created_by,
    const std::string &difficulty
) {
    const char* sql = "INSERT INTO tasks (title, description, difficulty, created_by) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return -1;
    }
    
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, difficulty.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, created_by);
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return -1;
    }
    
    int task_id = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);
    return task_id;

}

int TaskDB::addTask(const Task &task) {
    return addTask(task.title, task.description, task.ownerId, task.difficulty);
}

std::vector<Task> TaskDB::getAllTasks() {
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

std::optional<Task> TaskDB::getTaskById(int id) {
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

bool TaskDB::updateTask(int id, const Task &task, int user_id) {
    auto owner = getTaskOwner(id);
    if (!owner.has_value() || owner.value() != user_id) {
        return false; 
    }
    std::string sql = "UPDATE tasks SET title = '" + task.title +
                      "', description = '" + task.description +
                      "', difficulty = '" + task.difficulty +
                      "' WHERE id = " + std::to_string(id) + ";";
    return execute_SQL(sql);
}

bool TaskDB::deleteTask(int id, int user_id) {
    auto owner = getTaskOwner(id);
    if (!owner.has_value() || owner.value() != user_id) {
        return false;
    }
    std::string sql =
        "DELETE FROM tasks WHERE id = " + std::to_string(id) + ";";
    return execute_SQL(sql);
}

bool TaskDB::addTest(
    int task_id,
    const std::string &input,
    const std::string &expected
) {
    const char* sql = "INSERT INTO tests (task_id, input, expected_output) VALUES (?, ?, ?);";

    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, task_id);
    sqlite3_bind_text(stmt, 2, input.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, expected.c_str(), -1, SQLITE_STATIC);
    
    bool done = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return done;

}

std::vector<Test> TaskDB::getTestsForTask(int task_id) {
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
std::optional<int> TaskDB::getTaskOwner(int task_id) {
    std::string sql = "SELECT created_by FROM tasks WHERE id = " + std::to_string(task_id) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }
    std::optional<int> owner = std::nullopt;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        owner = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return owner;
}
