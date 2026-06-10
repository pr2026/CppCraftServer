#pragma once

#include <string>
#include <vector>
#include "RunResult.h"
#include "Test.h"

Run_result run_student_code(
    int user_id,
    int task_id,
    const std::string &code,
    const std::vector<Test> &tests
);

Run_result run_student_code_local(
    int user_id,
    int task_id,
    const std::string &code,
    const std::vector<Test> &tests
);

Run_result run_student_code_from_git(
    int user_id,
    int task_id,
    const std::string &git_url,
    const std::vector<Test> &tests
);