#include "StatisticsDB.h"
#include <iostream>

UserStatistics StatisticsDB::getUserStatistics(int user_id) {
    UserStatistics stats;
    std::string sql_total =
        "SELECT COUNT(*) as total_attempts, "
        "SUM(CASE WHEN passed_tests = total_tests THEN 1 ELSE 0 END) as solved "
        "FROM solutions WHERE user_id = " +
        std::to_string(user_id) + ";";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql_total.c_str(), -1, &stmt, nullptr) ==
        SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats.total_attempts = sqlite3_column_int(stmt, 0);
            stats.solved_tasks = sqlite3_column_int(stmt, 1);
        }
    }
    sqlite3_finalize(stmt);
    std::string sql_avg =
        "SELECT AVG(CAST(passed_tests AS REAL) / total_tests) * 100 "
        "FROM solutions WHERE user_id = " +
        std::to_string(user_id) + ";";
    if (sqlite3_prepare_v2(db, sql_avg.c_str(), -1, &stmt, nullptr) ==
        SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats.avg_success_rate = sqlite3_column_double(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    std::string sql_per_task =
        "SELECT task_id, "
        "COUNT(*) as attempts, "
        "MAX(passed_tests) as best, "
        "MAX(total_tests) as total_tests "
        "FROM solutions WHERE user_id = " +
        std::to_string(user_id) +
        " "
        "GROUP BY task_id;";
    if (sqlite3_prepare_v2(db, sql_per_task.c_str(), -1, &stmt, nullptr) ==
        SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            TaskStatistics task_stat;
            task_stat.task_id = sqlite3_column_int(stmt, 0);
            task_stat.attempt = sqlite3_column_int(stmt, 1);
            task_stat.best_result = sqlite3_column_int(stmt, 2);
            task_stat.total_tests = sqlite3_column_int(stmt, 3);
            task_stat.is_solved =
                (task_stat.best_result == task_stat.total_tests);
            stats.per_task.push_back(task_stat);
        }
    }
    sqlite3_finalize(stmt);
    return stats;
}