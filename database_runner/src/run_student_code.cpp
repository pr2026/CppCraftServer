#include "run_student_code.h"
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include "database.h"
#include "terminal.h"

namespace fs = std::filesystem;

test

    Run_result
    run_student_code(const std::string &code, const std::vector<Test> &tests) {
    Run_result result;
    // generate unique name
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()
    )
                  .count();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    int random_num = dis(gen);
    std::string unique_id =
        std::to_string(ms) + "_" + std::to_string(random_num);
    std::string filename = "/tmp/solution_" + unique_id + ".cpp";
    std::string executable = "/tmp/solution_" + unique_id;

    // save to file
    std::ofstream file(filename);
    if (!file) {
        result.pass_compile = false;
        result.compile_error = "ERROR: Cannot create file";
        return result;
    }
    file << code;
    file.close();

    if (!fs::exists(filename)) {
        result.pass_compile = false;
        result.compile_error = "ERROR: File was not created";
        return result;
    }

    if (fs::file_size(filename) == 0) {
        result.pass_compile = false;
        result.compile_error = "ERROR: File is empty";
        return result;
    }
    // compiletion
    std::string compile_command =
        "docker run --rm "
        "--memory=256m "
        "--cpus=0.5 "
        "--stop-timeout=5 "
        "--network=none "
        "--read-only "
        "-v /tmp:/workspace "
        "-w /workspace "
        "arm64v8/gcc:latest g++ /workspace/solution_" +
        unique_id + ".cpp -o solution_" + unique_id + " 2>&1";
    Terminal_result compile_res = terminal(compile_command);
    if (!fs::exists(executable)) {
        result.pass_compile = false;
        result.compile_error = compile_res.output;
        fs::remove(filename);
        return result;
    }

    result.pass_compile = true;
    result.compile_error = "";

    // pusk tests
    std::string run_result =
        "docker run --rm -i "
        "--memory=256m "
        "--cpus=0.5 "
        "--network=none "
        "--read-only "
        "--stop-timeout=5 "
        "-v /tmp:/workspace "
        "-w /workspace "
        "arm64v8/gcc:latest ./solution_" +
        unique_id;
    result.total_tests = tests.size();
    result.passed_tests = 0;
    std::vector<Test> test_results = tests;

    for (int i = 0; i < tests.size(); ++i) {
        std::cout << "Запуск теста " << i << " с input: '" << tests[i].input
                  << "'" << std::endl;
        Terminal_result run_res = terminal(run_result, tests[i].input);

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

    fs::remove(executable);
    fs::remove(filename);

    // save to database
    Database db("results.db");
    std::string sql =
        "INSERT INTO solutions (code, pass_compile, compile_error, "
        "total_tests, passed_tests) VALUES ('" +
        code + "', " + std::to_string(result.pass_compile) + ", '" +
        result.compile_error + "', " + std::to_string(result.total_tests) +
        ", " + std::to_string(result.passed_tests) + ");";
    if (db.executeSQL(sql)) {
        std::cout << "Результат сохранён в БД" << std::endl;
    } else {
        std::cerr << "Не удалось сохранить результат в БД" << std::endl;
    }
    return result;
}