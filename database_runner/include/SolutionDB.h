#pragma once
#include <string>
#include <vector>
#include "Database.h"

struct TaskStatistics {
    int task_id = 0;
    std::string task_title; 
    int attempt = 0;
    int best_result = 0;
    int total_tests = 0;
    bool is_solved = false;
};

struct UserStatistics {
    int total_attempts = 0;
    int solved_tasks = 0;
    double avg_success_rate = 0.0;
    std::vector<TaskStatistics> per_task;
};

struct TeacherTaskStatistics {
    int task_id;
    std::string title;
    int attempted_users_count; 
    int solved_users_count;    
    double avg_success_rate;     
};

class StatisticsDB : public Database {
public:
    using Database::Database;
    UserStatistics getUserStatistics(int user_id);
    std::vector<TeacherTaskStatistics> getTeacherTaskStatistics(int teacher_id);

};
