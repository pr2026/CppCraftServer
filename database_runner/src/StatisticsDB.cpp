#include "StatisticsDB.h"
#include <iostream>

UserStatistics StatisticsDB::getUserStatistics(int user_id) {
    UserStatistics stats;
    sqlite3_stmt *stmt;

    std::string sql_total =
        "SELECT COUNT(*) as total_attempts, "
        "SUM(CASE WHEN passed_tests = total_tests THEN 1 ELSE 0 END) as solved "
        "FROM solutions WHERE user_id = ?;";

    if (sqlite3_prepare_v2(db, sql_total, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats.total_attempts = sqlite3_column_int(stmt, 0);
            stats.solved_tasks = sqlite3_column_int(stmt, 1);
        }
    }
    sqlite3_finalize(stmt);

    std::string sql_avg =
        "SELECT AVG(CAST(passed_tests AS REAL) / total_tests) * 100 "
        "FROM solutions WHERE user_id = ?;";
    if (sqlite3_prepare_v2(db, sql_avg, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats.avg_success_rate = sqlite3_column_double(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);

    std::string sql_per_task =
        "SELECT task_id, t.title, "
        "COUNT(*) as attempts, "
        "MAX(passed_tests) as best, "
        "MAX(total_tests) as total_tests "
        "FROM solutions s JOIN tasks t ON s.task_id = t.id "
        "WHERE s.user_id = ? "
        "GROUP BY s.task_id;";

    if (sqlite3_prepare_v2(db, sql_per_task, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            TaskStatistics ts;
            ts.task_id = sqlite3_column_int(stmt, 0);
            ts.task_title = (const char *)sqlite3_column_text(stmt, 1);
            ts.attempt = sqlite3_column_int(stmt, 2);
            ts.best_result = sqlite3_column_int(stmt, 3);
            ts.total_tests = sqlite3_column_int(stmt, 4);
            ts.is_solved = (ts.best_result == ts.total_tests);
            stats.per_task.push_back(ts);
        }
    }
    sqlite3_finalize(stmt);
    return stats;
}

std::vector<TeacherTaskStats> StatisticsDB::getTeacherTaskStatistics(
    int teacher_id
) {
    std::vector<TeacherTaskStats> results;
    sqlite3_stmt *stmt;

    const char *sql =
        "SELECT t.id, t.title, "
        "COUNT(DISTINCT s.user_id), "
        "COUNT(DISTINCT CASE WHEN s.passed_tests = s.total_tests THEN "
        "s.user_id END), "
        "IFNULL(AVG(CAST(s.passed_tests AS REAL) / s.total_tests) * 100, 0.0) "
        "FROM tasks t "
        "LEFT JOIN solutions s ON t.id = s.task_id "
        "WHERE t.created_by = ? "
        "GROUP BY t.id;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, teacher_id);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            TeacherTaskStats row;
            row.task_id = sqlite3_column_int(stmt, 0);
            row.title = ((const char *)sqlite3_column_text(stmt, 1));
            row.attempted_users_count = sqlite3_column_int(stmt, 2);
            row.solved_users_count = sqlite3_column_int(stmt, 3);
            row.success_rate = sqlite3_column_double(stmt, 4);
            results.push_back(row);
        }
    }

    sqlite3_finalize(stmt);
    return results;
}
