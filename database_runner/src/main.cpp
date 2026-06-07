#include <iostream>
#include <vector>
#include "Database.h"
#include "SolutionDB.h"
#include "StatisticsDB.h"
#include "TaskDB.h"
#include "UserDB.h"
#include "run_student_code.h"
#include "saveStudentsCode.h"

int main() {
    UserDB users("cppcraft.db");
    TaskDB tasks("cppcraft.db");
    SolutionDB solutions("cppcraft.db");
    StatisticsDB stat_db("cppcraft.db");

    int user_id = -1;
    if (users.addUser("alice", "123", "student")) {
        auto id = users.get_user_id("alice");
        if (id.has_value()) {
            std::cout << "Пользователь alice создан с ID: " << id.value() << std::endl;
        }
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

    // save solutions

    std::string code_path = SaveStudentCode(user_id, task_id, students_kod);
    if (solutions.addSolution(user_id, task_id, code_path, result)) {
        std::cout << "\nРешение сохранено в БД" << std::endl;
    } else {
        std::cerr << "Ошибка сохранения решения" << std::endl;
    }

    std::cout << "\n=== ИСТОРИЯ РЕШЕНИЙ ===\n";
    auto history = solutions.getSolutionsForUser(user_id);
    for (const auto &sol : history) {
        std::cout << "Решение #" << sol.id << ": задача " << sol.task_id
                  << " → " << sol.result.passed_tests << "/"
                  << sol.result.total_tests;
        if (sol.result.passed_tests == sol.result.total_tests) {
            std::cout << "done";
        }
        std::cout << std::endl;
    }
    // ===== СТАТИСТИКА ПОЛЬЗОВАТЕЛЯ =====
    
    UserStatistics stats = stat_db.getUserStatistics(user_id);
    std::cout << "\n=== СТАТИСТИКА ПОЛЬЗОВАТЕЛЯ ===\n";
    std::cout << "Всего попыток: " << stats.total_attempts << std::endl;
    std::cout << "Решено задач: " << stats.solved_tasks << std::endl;
    std::cout << "Средний успех: " << stats.avg_success_rate << "%" << std::endl;

    if (!stats.per_task.empty()) {
        std::cout << "\nИнформация по задачам:\n";
        for (const auto& task : stats.per_task) {
            std::cout << "  Задача " << task.task_id;
            if (!task.task_title.empty()) {
                std::cout << " (\"" << task.task_title << "\")";
            }
            std::cout << ": " << task.attempt << " попыток, "
                      << "лучший результат " << task.best_result << "/" << task.total_tests;
            if (task.is_solved) {
                std::cout << " ✅";
            }
            std::cout << std::endl;
        }
    }
}