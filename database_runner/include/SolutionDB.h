#pragma once
#include <string>
#include "RunResult.h"
<<<<<<< HEAD
#include "Database.h"
#include <vector>
#include <sqlite3.h>
#include <string>
=======
>>>>>>> 8d73483877d337adbc4c45cb4dafcffde3789106

struct Solution {
    int id;
    int user_id;
    int task_id;
    std::string code_path;
    Run_result result;
<<<<<<< HEAD
    std::string created_at;
=======
>>>>>>> 8d73483877d337adbc4c45cb4dafcffde3789106

    Solution() : id(0), user_id(0), task_id(0) {
    }
};

class SolutionDB : public Database {
public:
<<<<<<< HEAD
    using Database::Database;
    bool addSolution(
        int user_id,
        int task_id,
        const std::string &code_path,
        const Run_result &result
    );
    std::vector<Solution> getSolutionsForUser(int user_id);
    std::vector<Solution> getSolutionsForTask(int task_id);
};
=======
    using Database::Database; 
    bool addSolution(int user_id, int task_id, const std::string& code_path, const Run_result& result);
    std::vector<Solution> getSolutionsForUser(int user_id);
    std::vector<Solution> getSolutionsForTask(int task_id);
}
>>>>>>> 8d73483877d337adbc4c45cb4dafcffde3789106
