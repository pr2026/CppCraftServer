#pragma once

#include <string>

struct Terminal_result {
    std::string output;
    std::string error_msg;
    int exit_code;
};

Terminal_result
terminal(const std::string &command, const std::string &input = "");