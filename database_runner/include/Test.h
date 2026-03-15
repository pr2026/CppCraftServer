#pragma once
#include <string>

struct Test {
    int id = -1;
    int task_id = -1;
    std::string input;
    std::string expected_output;
    std::string real_output;
    bool passed = false;

    Test() : id(0), task_id(0), passed(false) {
    }

    Test(
        int id,
        int task_id,
        const std::string &input,
        const std::string &expected_output
    )
        : id(id),
          task_id(task_id),
          input(input),
          expected_output(expected_output),
          passed(false) {
    }

    Test(const std::string &input, const std::string &expected_output)
        : id(-1),
          task_id(-1),
          input(input),
          expected_output(expected_output),
          passed(false) {
    }
};