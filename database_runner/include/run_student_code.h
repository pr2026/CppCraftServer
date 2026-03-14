#pragma once

#include "run_result.h"
#include "Test.h"
#include <string>
#include <vector>

Run_result run_student_code(const std::string &code,
                            const std::vector<Test> &tests);
