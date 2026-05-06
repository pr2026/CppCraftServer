#include <iostream>
#include <vector>
#include "Database.h"
#include "TaskDB.h"
#include "UserDB.h"
#include "run_student_code.h"
#include "StatisticsDB.h"

int main() {
    UserDB users("cppcraft.db");
    TaskDB tasks("cppcraft.db");

    int user_id = users.addUser("alice", "123", "student");
    if (user_id != -1) {
        std::cout << "Пользователь alice создан с ID: " << user_id << std::endl;
    } else {
        std::cout << "Пользователь уже существует" << std::endl;
    }

    bool login = users.checkPassword("alice", "123");
    if (login) {
        std::cout << "Вы вошли!" << std::endl;
    } else {
        std::cout << "Неверный пароль" << std::endl;
    }

    int task_id = tasks.addTask("Сумма чисел", "Сложи два числа", "easy");
    std::cout << "Создана задача с ID: " << task_id << std::endl;

    tasks.addTest(task_id, "20 22", "42");
    std::cout << "Добавлен тест" << std::endl;

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
    auto tests = tasks.getTestsForTask(task_id);
    Run_result result = run_student_code(user_id, task_id, students_kod, tests);

    if (!result.pass_compile) {
        std::cout << "COMPILATION ERROR:\n"
                  << result.compile_error << std::endl;
        return 1;
    }

    std::cout << "Running " << result.total_tests << " tests...\n";
    for (int i = 0; i < result.tests_results.size(); ++i) {
        std::cout << "Test " << i + 1 << ": ";

        if (result.tests_results[i].passed) {
            std::cout << "PASSED\n";
        } else {
            std::cout << "FAILED\n";
            std::cout << "Expected: '"
                      << result.tests_results[i].expected_output << "'\n";
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