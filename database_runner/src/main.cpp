#include "run_student_code.h"
#include <iostream>
#include <vector>

int main() {
  std::string students_kod = R"(
#include <iostream>
int main() {
    int a, b;
    if (!(std::cin >> a >> b)) {
        std::cout << "NO_INPUT";
        return 1;
    }
    std::cout << a + b;
    return 0;
}
)";
  std::vector<Test> tests = { {"20 22", "42", "", false} };

  Run_result result = run_student_code(students_kod, tests);

  if (!result.pass_compile) {
    std::cout << "COMPILATION ERROR:\n" << result.compile_error << std::endl;
    return 1;
  }

  std::cout << "Running " << result.total_tests << " tests...\n";
  for (int i = 0; i < result.tests_results.size(); ++i) {

    std::cout << "Test " << i + 1 << ": ";

    if (result.tests_results[i].passed) {
      std::cout << "PASSED\n";
    } else {
      std::cout << "FAILED\n";
      std::cout << "Expected: '" << result.tests_results[i].expected_output
                << "'\n";
      std::cout << "Got:      '" << result.tests_results[i].real_output
                << "'\n";
    }
  }

  std::cout << "\n=== RESULTS ===\n";
  std::cout << "Passed: " << result.passed_tests << "/" << result.total_tests
            << " tests\n";

  if (result.passed_tests == result.total_tests) {
    std::cout << "ALL TESTS PASSED!\n";
  } else {
    std::cout << "Some tests failed.\n";
  }
}