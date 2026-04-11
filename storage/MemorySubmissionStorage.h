#pragma once
#include <mutex>
#include <unordered_map>
#include "SubmissionStorage.h"

class MemorySubmissionStorage : public SubmissionStorage {
public:
    MemorySubmissionStorage();
    int addSubmission(int userId, int taskId, const std::string &code) override;
    std::optional<Submission> getSubmissionById(int id) override;
    bool updateSubmissionResult(int id, const SubmissionResult &result)
        override;

private:
    std::unordered_map<int, Submission> submissions;
    std::mutex m;
    int nextId = 1;
};