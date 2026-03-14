#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "run_student_code.h"
#include <vector>
#include <string>

TEST_CASE("Успешная компиляция") {
    std::string code = R"(
        #include <iostream>
        int main() {
            std::cout << "Hello world!";
            return 0;
        }
    )";
    
    std::vector<Test> tests = {
        {"", "Hello world!", "", false}
    };
    Run_result result = run_student_code(code, tests);

    CHECK(result.pass_compile == true);
    CHECK(result.tests_results.size() == 1);
    CHECK(result.tests_results[0].passed == true);
    CHECK(result.tests_results[0].real_output == "Hello world!");
}

TEST_CASE("Ошибка компиляции") {
    std::string code = R"(
        #include <iostream>
        int main() {
            std::cout << "Hello"
            return 0;
        }
    )";
    
    std::vector<Test> tests = {};
    Run_result result = run_student_code(code, tests);

    CHECK(result.pass_compile == false);
    CHECK(result.compile_error.find("error") != std::string::npos);
}

TEST_CASE("Cумма чисел") {
    std::string code = R"(
        #include <iostream>
        int main() {
            int a, b;
            std::cin >> a >> b;
            std::cout << a + b;
            return 0;
        }
    )";
    
    std::vector<Test> tests = {
        {"2 3", "5", "", false},
        {"10 20", "30", "", false},
        {"-5 5", "0", "", false},
        {"100 200", "300", "", false}
    };
    
    Run_result result = run_student_code(code, tests);
    
    CHECK(result.pass_compile == true);
    CHECK(result.total_tests == 4);
    CHECK(result.passed_tests == 4);
    
    for (const auto& test : result.tests_results) {
        CHECK(test.passed == true);
    }
}

TEST_CASE("Ошибка выполнения") {
    std::string code = R"(
        #include <iostream>
        int main() {
            int x = 5;
            int y = x / 0;
            std::cout << y;
            return 0;
        }
    )";
    
    std::vector<Test> tests = {{"", "", "", false}};
    
    Run_result result = run_student_code(code, tests);
    
    CHECK(result.pass_compile == true);
    CHECK(result.tests_results[0].passed == false);
}