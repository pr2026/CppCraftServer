#include "MemorySubmissionStorage.h"

MemorySubmissionStorage::MemorySubmissionStorage() {}

int MemorySubmissionStorage::addSubmission(int userId, int taskId, const std::string& code) {
    std::lock_guard<std::mutex> lock(m);
    Submission sub;
    sub.id = nextId++;
    sub.userId = userId;
    sub.taskId = taskId;
    sub.code = code;
    sub.result = SubmissionResult{};
    sub.submittedAt = std::chrono::system_clock::now();
    submissions[sub.id] = sub;
    return sub.id;
}

std::optional<Submission> MemorySubmissionStorage::getSubmissionById(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = submissions.find(id);
    if (it == submissions.end()) return std::nullopt;
    return it->second;
}

bool MemorySubmissionStorage::updateSubmissionResult(int id, const SubmissionResult& result) {
    std::lock_guard<std::mutex> lock(m);
    auto it = submissions.find(id);
    if (it == submissions.end()) return false;
    it->second.result = result;
    return true;
}