#include "SubmissionController.h"
#include <drogon/drogon.h>
#include "storage/MemorySubmissionStorage.h"
#include "storage/MemoryTaskStorage.h"

SubmissionController::SubmissionController()
    : taskStorage(std::make_unique<MemoryTaskStorage>()),
      submissionStorage(std::make_unique<MemorySubmissionStorage>()) {
    std::cout << "SubmissionController created" << std::endl;
}

void SubmissionController::submitSolution(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    int taskId
) {
    std::cout << "1. submitSolution START" << std::endl;
    auto taskOpt = taskStorage->getTaskById(taskId);
    if (!taskOpt.has_value()) {
        Json::Value result;
        result["error"] = "Task not found";
        auto result_callback =
            drogon::HttpResponse::newHttpJsonResponse(result);
        result_callback->setStatusCode(drogon::k404NotFound);
        callback(result_callback);
        return;
    }

    const auto &task = taskOpt.value();
    auto jsonPtr = req->getJsonObject();
    if (!jsonPtr || !jsonPtr->isMember("code")) {
        Json::Value result;
        result["error"] = "Missing or invalid JSON, field 'code' required";
        auto result_callback =
            drogon::HttpResponse::newHttpJsonResponse(result);
        result_callback->setStatusCode(drogon::k404NotFound);
        callback(result_callback);
        return;
    }

    std::string code = (*jsonPtr)["code"].asString();
    int userId = 1;  // in the future need to change
    int submissionId = submissionStorage->addSubmission(userId, taskId, code);

    std::vector<Test> runnerTests;
    for (const auto &tc : task.tests) {
        Test t;
        t.input = tc.input;
        t.expected_output = tc.expected_output;
        runnerTests.push_back(t);
    }
    std::cout << "2. Before calling runner" << std::endl;
    Run_result runnerResult = run_student_code(code, runnerTests);
    std::cout << "3. After runner" << std::endl;

    SubmissionResult subResult;
    subResult.compilationSuccess = runnerResult.pass_compile;
    subResult.compileError = runnerResult.compile_error;
    subResult.totalTests = runnerResult.total_tests;
    subResult.passedTests = runnerResult.passed_tests;

    submissionStorage->updateSubmissionResult(submissionId, subResult);
    Json::Value result;
    result["submission_id"] = submissionId;

    if (!runnerResult.pass_compile) {
        result["status"] = "compilation_error";
        result["compile_error"] = runnerResult.compile_error;
    } else {
        result["status"] = "finished";
        result["passed_tests"] = runnerResult.passed_tests;
        result["total_tests"] = runnerResult.total_tests;
        result["verdict"] =
            (runnerResult.passed_tests == runnerResult.total_tests)
                ? "OK"
                : "Wrong Answer";
    }

    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    callback(result_callback);
}

void SubmissionController::getSubmission(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    int submissionId
) {
    auto subOpt = submissionStorage->getSubmissionById(submissionId);
    if (!subOpt.has_value()) {
        Json::Value result;
        result["error"] = "Task not found";
        auto result_callback =
            drogon::HttpResponse::newHttpJsonResponse(result);
        result_callback->setStatusCode(drogon::k404NotFound);
        callback(result_callback);
        return;
    }

    const auto &sub = subOpt.value();
    Json::Value result;
    result["submission_id"] = sub.id;
    result["user_id"] = sub.userId;
    result["task_id"] = sub.taskId;
    result["code"] = sub.code;
    result["compilation_success"] = sub.result.compilationSuccess;
    result["compile_error"] = sub.result.compileError;
    result["total_tests"] = sub.result.totalTests;
    result["passed_tests"] = sub.result.passedTests;

    auto resp = drogon::HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}