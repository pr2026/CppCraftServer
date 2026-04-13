#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include "Unique.h"

namespace fs = std::filesystem;

inline std::string
SaveStudentCode(int user_id, int task_id, const std::string &code) {
    // create folder
    if (!fs::exists("student_codes")) {
        fs::create_directory("student_codes");
    }
    // generate unique name
    std::string filename = "student_codes/user_" + std::to_string(user_id) +
                           "_task_" + std::to_string(task_id) + "_" +
                           GenerateUniqueId() + ".cpp";
    // save code
    std::ofstream file(filename);
    file << code;
    file.close();

    return filename;
}