#pragma once
#include <drogon/drogon.h>
#include "storage/TaskStorage.h"
#include "storage/SubmissionStorage.h"
#include "database_runner/include/run_student_code.h"
#include <memory>

class SubmissionController : public drogon::HttpController<SubmissionController>
{
  public:
    SubmissionController();
    void submitSolution(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback, int taskId);
    void getSubmission(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback, int submissionId);
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(SubmissionController::submitSolution, "/tasks/{task-id}/submit", drogon::Post);
        ADD_METHOD_TO(SubmissionController::getSubmission, "/submissions/{submission-id}", drogon::Get);
    METHOD_LIST_END

  private:
    std::unique_ptr<TaskStorage> taskStorage;
    std::unique_ptr<SubmissionStorage> submissionStorage;
};