#pragma once
#include <string>
#include "Database.h"
#include "run_result.h"

inline bool save_result_to_db(
    Database &db,
    int user_id,
    int task_id,
    const std::string &code_path,
    const Run_result &result
) {
    std::string sql =
        "INSERT INTO solutions (user_id, task_id, code, pass_compile, "
        "compile_error, total_tests, passed_tests) VALUES (" +
        std::to_string(user_id) + ", " + std::to_string(task_id) + ", '" +
        code_path + "', " + std::to_string(result.pass_compile) + ", '" +
        result.compile_error + "', " + std::to_string(result.total_tests) +
        ", " + std::to_string(result.passed_tests) + ");";

    return db.execute_SQL(sql);
}