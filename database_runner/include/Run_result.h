#pragma once
#include <string>
#include <vector>
#include "test.h"

struct Run_result {
    bool pass_compile = false;
    std::string compile_error;

    int total_tests = 0;
    int passed_tests = 0;
    std::vector<Test> tests_results;
    int exit_code = 0;
};