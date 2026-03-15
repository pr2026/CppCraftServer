#pragma once

#include <string>
#include <vector>
#include "run_result.h"
#include "test.h"

Run_result run_student_code(
    int user_id,
    int task_id,
    const std::string &code,
    const std::vector<Test> &tests
);
