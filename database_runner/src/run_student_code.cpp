#include "code_runner.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <random>

namespace fs = std::filesystem;

Run_result run_student_code(const std::string &code,
                            const std::vector<Test> &tests) {
  Run_result result;

  auto now = std::chrono::system_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch())
                .count();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1000, 9999);
  int random_num = dis(gen);

  std::string filename = "/tmp/solution_" + std::to_string(ms) + "_" +
                         std::to_string(random_num) + ".cpp";

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

  std::string compile_command = "docker run --rm "
                                "--memory=256m "
                                "--cpus=0.5 "
                                "--stop-timeout=5 "
                                "--network=none "
                                "--read-only "
                                "-v /tmp:/workspace "
                                "-w /workspace "
                                "silkeh/clang:latest clang++ " +
                                filename + " -o solution 2>&1";

  std::string compile_result = terminal(compile_command);
  std::string executable = "/tmp/solution";

  if (!fs::exists(executable)) {
    result.pass_compile = false;
    result.compile_error = compile_result;
    fs::remove(filename);
    return result;
  }
  result.pass_compile = true;
  result.compile_error = "";
  std::string run_result = "docker run --rm "
                           "--memory=256m "
                           "--cpus=0.5 "
                           "--network=none "
                           "--read-only "
                           "--stop-timeout=5 "
                           "-v /tmp:/workspace "
                           "-w /workspace "
                           "silkeh/clang:latest ./solution";

  result.total_tests = tests.size();
  result.passed_tests = 0;
  std::vector<Test> test_results = tests;

  for (int i = 0; i < tests.size(); ++i) {

    std::string output = terminal(run_result, tests[i].input);

    test_results[i].real_output = output;
    if (output == tests[i].expected_output) {
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
  return result;
}