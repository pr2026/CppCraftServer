// #pragma once
// #include <string>
// #include "Database.h"
// #include "RunResult.h"

// inline bool save_result_to_db(
//     Database &db,
//     int user_id,
//     int task_id,
//     const std::string &code_path,
//     const Run_result &result
// ) {
//     std::string sql =
//         "INSERT INTO solutions (user_id, task_id, code_path, pass_compile, "
//         "compile_error, total_tests, passed_tests) VALUES (" +
//         std::to_string(user_id) + ", " + std::to_string(task_id) + ", '" +
//         code_path + "', " + std::to_string(result.pass_compile) + ", '" +
//         result.compile_error + "', " + std::to_string(result.total_tests) +
//         ", " + std::to_string(result.passed_tests) + ");";

//     return db.execute_SQL(sql);
// }



#pragma once
#include <string>
#include "Database.h"
#include "RunResult.h"

inline std::string escape_sql(const std::string& s) {
    std::string r;
    for (char c : s) {
        if (c == '\'') r += "''";
        else r += c;
    }
    return r;
}

inline bool save_result_to_db(Database &db, int user_id, int task_id, const std::string &code_path, const Run_result &result) {
    std::string safe_path = escape_sql(code_path);
    std::string safe_error = escape_sql(result.compile_error);
    std::string sql = 
        "INSERT INTO solutions (user_id, task_id, code_path, pass_compile, compile_error, total_tests, passed_tests) VALUES (" +
        std::to_string(user_id) + ", " + std::to_string(task_id) + ", '" +
        safe_path + "', " + std::to_string(result.pass_compile) + ", '" +
        safe_error + "', " + std::to_string(result.total_tests) +
        ", " + std::to_string(result.passed_tests) + ");";
    return db.execute_SQL(sql);
}