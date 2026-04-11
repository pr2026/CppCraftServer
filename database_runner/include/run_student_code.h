#pragma once

#include <string>
#include <vector>
#include "Run_result.h"
#include "Test.h"

Run_result
run_student_code(const std::string &code, const std::vector<Test> &tests);
