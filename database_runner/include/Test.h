#pragma once
#include <string>

struct Test {
  std::string input;
  std::string expected_output;
  std::string real_output;
  bool passed = false;
};