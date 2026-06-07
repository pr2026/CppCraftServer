#include "SolutionDB.h"
#include <iostream>

bool SolutionDB::addSolution(
    int user_id,
    int task_id,
    const std::string &code_path,
    const Run_result &result
) {
    std::string sql =
        "INSERT INTO solutions (user_id, task_id, code_path, pass_compile, "
        "compile_error, total_tests, passed_tests) VALUES (?, ?, ?, ?, ?, ?, "
        "?)";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, task_id);
    sqlite3_bind_text(stmt, 3, code_path.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, result.pass_compile ? 1 : 0);
    sqlite3_bind_text(stmt, 5, result.compile_error.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, result.total_tests);
    sqlite3_bind_int(stmt, 7, result.passed_tests);
    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

std::vector<Solution> SolutionDB::getSolutionsForUser(int user_id) {
    std::vector<Solution> solutions;
    std::string sql =
        "SELECT id, user_id, task_id, code_path, pass_compile, compile_error, "
        "total_tests, passed_tests, created_at FROM solutions WHERE user_id = "
        "? ORDER BY created_at DESC;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return solutions;
    }
    sqlite3_bind_int(stmt, 1, user_id);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Solution sol;
        sol.id = sqlite3_column_int(stmt, 0);
        sol.user_id = sqlite3_column_int(stmt, 1);
        sol.task_id = sqlite3_column_int(stmt, 2);
        sol.code_path = (const char *)sqlite3_column_text(stmt, 3);
        sol.result.pass_compile = sqlite3_column_int(stmt, 4);
        sol.result.compile_error = (const char *)sqlite3_column_text(stmt, 5);
        sol.result.total_tests = sqlite3_column_int(stmt, 6);
        sol.result.passed_tests = sqlite3_column_int(stmt, 7);
        sol.created_at = (const char *)sqlite3_column_text(stmt, 8);
        solutions.push_back(sol);
    }
    sqlite3_finalize(stmt);
    return solutions;
}

std::vector<Solution> SolutionDB::getSolutionsForTask(int task_id) {
    std::vector<Solution> solutions;
    std::string sql =
        "SELECT id, user_id, task_id, code_path, pass_compile, compile_error, "
        "total_tests, passed_tests, created_at FROM solutions WHERE task_id = "
        "? ORDER BY created_at DESC;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return solutions;
    }
    sqlite3_bind_int(stmt, 1, task_id);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Solution sol;
        sol.id = sqlite3_column_int(stmt, 0);
        sol.user_id = sqlite3_column_int(stmt, 1);
        sol.task_id = sqlite3_column_int(stmt, 2);
        sol.code_path = (const char *)sqlite3_column_text(stmt, 3);
        sol.result.pass_compile = sqlite3_column_int(stmt, 4);
        sol.result.compile_error = (const char *)sqlite3_column_text(stmt, 5);
        sol.result.total_tests = sqlite3_column_int(stmt, 6);
        sol.result.passed_tests = sqlite3_column_int(stmt, 7);
        sol.created_at = (const char *)sqlite3_column_text(stmt, 8);
        solutions.push_back(sol);
    }
    sqlite3_finalize(stmt);
    return solutions;
}