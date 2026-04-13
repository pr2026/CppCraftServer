#pragma once
#include <string>
#include "RunResult.h"

struct Solution {
    int id;
    int user_id;
    int task_id;
    std::string code;
    Run_result result;

    Solution() : id(0), user_id(0), task_id(0) {
    }
};