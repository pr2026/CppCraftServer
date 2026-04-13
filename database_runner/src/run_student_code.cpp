#include "run_student_code.h"
#include <filesystem>
#include <iostream>
#include "Database.h"
#include "Docker.h"
#include "Test.h"
#include "Unique.h"
#include "saveCodeToDatabase.h"
#include "saveStudentsCode.h"

namespace fs = std::filesystem;

Run_result run_student_code(
    int user_id,
    int task_id,
    const std::string &code,
    const std::vector<Test> &tests
) {
    Run_result result;  // output our solve

    std::string unique_id = GenerateUniqueId();

    CreateTempFile(unique_id, code);  // realse in docker.h

    Terminal_result compile_res = compile(unique_id);

    std::string executable = "/tmp/solution_" + unique_id;
    if (!fs::exists(executable)) {
        result.pass_compile = false;
        result.compile_error = compile_res.output;  // its output from terminal
        RemoveTempFiles(unique_id);
        return result;
    }

    result.pass_compile = true;
    result.compile_error = "";

    // tests
    result.total_tests = tests.size();
    result.passed_tests = 0;
    std::vector<Test> test_results = tests;

    for (int i = 0; i < tests.size(); ++i) {
        std::cout << "Запуск теста " << i << " с input: '" << tests[i].input
                  << "'" << std::endl;
        Terminal_result run_res = run(unique_id, tests[i].input);

        std::cout << "Вывод программы: '" << run_res.output << "'" << std::endl;

        test_results[i].real_output = run_res.output;

        if (run_res.exit_code != 0) {
            test_results[i].passed = false;
            test_results[i].real_output +=
                "\n[ОШИБКА: " + run_res.error_msg + "]";
        } else if (run_res.output == tests[i].expected_output) {
            test_results[i].passed = true;
        } else {
            test_results[i].passed = false;
        }

        if (test_results[i].passed) {
            result.passed_tests++;
        }
    }

    result.tests_results = test_results;
    std::string code_path = SaveStudentCode(user_id, task_id, code);
    RemoveTempFiles(unique_id);

    // save in db
    Database db("cppcraft.db");
    save_result_to_db(db, user_id, task_id, code_path, result);

    return result;
}