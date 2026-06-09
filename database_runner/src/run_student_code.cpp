#include "run_student_code.h"
#include <filesystem>
#include <iostream>
#include <regex>
#include "Database.h"
#include "Docker.h"
#include "Test.h"
#include "Unique.h"
#include "saveCodeToDatabase.h"
#include "saveStudentsCode.h"

namespace fs = std::filesystem;

bool is_git_url(const std::string &url) {
    std::regex git_pattern(
        R"(^(https?://|git@)[a-zA-Z0-9.-]+(\.com|\.org|\.io|\.net|\.ru)[/:][a-zA-Z0-9_.-]+/[a-zA-Z0-9_.-]+(\.git)?$)"
    );
    return std::regex_match(url, git_pattern);
}

std::string clone_repo(const std::string &git_url, std::string &unique_id) {
    unique_id = GenerateUniqueId();
    std::string repo_path = "/tmp/repo_" + unique_id;
    std::string cmd =
        "git clone --depth 1 " + git_url + " " + repo_path + " 2>&1";
    system(cmd.c_str());
    return repo_path;
}

std::vector<std::string> find_cpp_files(const std::string &folder_path) {
    std::vector<std::string> cpp_files;

    for (const auto &entry : fs::recursive_directory_iterator(folder_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
            cpp_files.push_back(entry.path().string());
        }
    }
    return cpp_files;
}

Run_result run_student_code(
    int user_id,
    int task_id,
    const std::string &code,
    const std::vector<Test> &tests
) {
    if (is_git_url(code)) {
        return run_student_code_from_git(user_id, task_id, code, tests);
    } else {
        return run_student_code_local(user_id, task_id, code, tests);
    }
}

Run_result run_student_code_from_git(
    int user_id,
    int task_id,
    const std::string &git_url,
    const std::vector<Test> &tests
) {
    Run_result result;
    std::string unique_id;
    std::string repo_path = clone_repo(git_url, unique_id);
    std::vector<std::string> cpp_files = find_cpp_files(repo_path);

    std::string compile_command =
        "docker run --rm "
        "--memory=256m --cpus=0.5 --network=none --read-only "
        "-v " +
        repo_path +
        ":/workspace "
        "-w /workspace "
        "gcc:latest bash -c 'g++";

    for (const auto &file : cpp_files) {
        std::string rel_path = fs::relative(file, repo_path).string();
        compile_command += " " + rel_path;
    }

    compile_command += " -o solution 2>&1'";

    Terminal_result compile_res = terminal(compile_command);
    std::string executable = repo_path + "/solution";

    if (!fs::exists(executable)) {
        result.pass_compile = false;
        result.compile_error = compile_res.output;
        fs::remove_all(repo_path);
        return result;
    }

    result.pass_compile = true;
    result.compile_error = "";

    result.total_tests = tests.size();
    result.passed_tests = 0;
    std::vector<Test> test_results = tests;

    for (size_t i = 0; i < tests.size(); ++i) {
        std::string run_command =
            "docker run --rm -i "
            "--memory=256m --cpus=0.5 --network=none --read-only "
            "-v " +
            repo_path +
            ":/workspace "
            "-w /workspace "
            "gcc:latest ./solution";

        Terminal_result run_res = terminal(run_command, tests[i].input);

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

    std::string code_path =
        SaveStudentCode(user_id, task_id, "git_repo_" + unique_id);
    fs::create_directories(fs::path(code_path).parent_path());

    for (const auto &file : cpp_files) {
        fs::path dest = fs::path(code_path) / fs::path(file).filename();
        fs::copy(file, dest, fs::copy_options::overwrite_existing);
    }

    fs::remove_all(repo_path);

    Database db("cppcraft.db");
    save_result_to_db(db, user_id, task_id, code_path, result);

    return result;
}

Run_result run_student_code_local(
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
