#include "run_student_code.h"
#include "Unique.h"
#include "Docker.h"
#include "saveStudentsCode.h"
#include "saveCodeToDatabase.h"
#include "Database.h"
#include "test.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

Run_result run_student_code(
    int user_id,
    int task_id,
    const std::string &code,
    const std::vector<Test> &tests
) {
    Run_result result;


    // generate unique name
    std::string unique_id = generate_unique_id();
    
    create_temp_file(unique_id, code);

    Terminal_result compile_res = compile(unique_id);


    std::string executable = "/tmp/solution_" + unique_id;
    //check compile
    if (!fs::exists(executable)) {
        result.pass_compile = false;
        result.compile_error = compile_res.output;
        remove_temp_files(unique_id);
        return result;
    }

    result.pass_compile = true;
    result.compile_error = "";


    //work with tests
    result.total_tests = tests.size();
    result.passed_tests = 0;
    std::vector<Test> test_results = tests;

    for (int i = 0; i < tests.size(); ++i) {
        std::cout << "Запуск теста " << i << " с input: '" << tests[i].input
                  << "'" << std::endl;
        Terminal_result run_res = run(run_result, tests[i].input);

        std::cout << "Вывод программы: '" << run_res.output << "'" << std::endl;
        if (!run_res.error_msg.empty()) {
            std::cout << "ОШИБКА: " << run_res.error_msg << std::endl;
        }

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

    
    std::string code_path = save_student_code(user_id, task_id, code);
    //remove temps
    remove_temp_files(unique_id);
    
    //save in db
    Database db("cppcraft.db");
    save_result_to_db(db, user_id, task_id, code_path, result);

    return result;
}