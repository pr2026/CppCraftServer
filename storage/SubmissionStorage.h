#pragma once
#include <string>
#include <vector>
#include <optional>
#include <chrono>

struct SubmissionResult {
    bool compilationSuccess = false;
    std::string compileError;
    int totalTests = 0;
    int passedTests = 0;
};

struct Submission {
    int id;
    int userId;
    int taskId;
    std::string code;
    SubmissionResult result;
    std::chrono::system_clock::time_point submittedAt;
};

class SubmissionStorage {
public:
    virtual ~SubmissionStorage() = default;
    virtual int addSubmission(int userId, int taskId, const std::string& code) = 0;
    virtual std::optional<Submission> getSubmissionById(int id) = 0;
    virtual bool updateSubmissionResult(int id, const SubmissionResult& result) = 0;
};