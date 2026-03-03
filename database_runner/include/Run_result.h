#pragma once
#include "Test.h"
#include <string>
#include <vector>

struct Run_result {

  bool pass_compile = false;
  std::string compile_error;

  int total_tests = 0;
  int passed_tests = 0;
  std::vector<Test> tests_results;
  double execution_time = 0;
  int exit_code = 0;
};