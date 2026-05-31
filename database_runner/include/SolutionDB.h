#pragma once
#include <string>
#include "RunResult.h"

struct Solution {
    int id;
    int user_id;
    int task_id;
    std::string code_path;
    Run_result result;

    Solution() : id(0), user_id(0), task_id(0) {
    }
};

class SolutionDB : public Database {
public:
    using Database::Database; 
    bool addSolution(int user_id, int task_id, const std::string& code_path, const Run_result& result);
    std::vector<Solution> getSolutionsForUser(int user_id);
    std::vector<Solution> getSolutionsForTask(int task_id);
}
