#include "StatisticsController.h"
#include <drogon/drogon.h>

void StatisticsController::getUserStatistics(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback, int userId) {
    StatisticsDB db("cppcraft.db");
    UserStatistics stats = db.getUserStatistics(userId);

    Json::Value ret;
    ret["total_attempts"] = stats.total_attempts;
    ret["solved_tasks"] = stats.solved_tasks;
    ret["success_rate"] = stats.avg_success_rate;

    Json::Value tasksDetails(Json::arrayValue);
    for (const auto& taskInfo : stats.per_task) {
        Json::Value taskJson;
        taskJson["task_id"] = taskInfo.task_id;
        taskJson["task_title"] = taskInfo.task_title;
        taskJson["attempts"] = taskInfo.attempt;
        taskJson["best_result"] = taskInfo.best_result;
        taskJson["is_solved"] = taskInfo.is_solved;
        tasksDetails.append(taskJson);
    }
    ret["tasks_details"] = tasksDetails;

    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}