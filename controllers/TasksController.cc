#include "TasksController.h"

void TasksController::getTasks(const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback){
    auto tasks = taskStorage->getAllTasks();

    Json::Value tasksJson(Json::arrayValue);

    for (auto& task : tasks) {
        Json::Value t;
        t["id"] = task.id;
        t["title"] = task.title;
        t["description"] = task.description;
        t["difficulty"] = task.difficulty;
        tasksJson.append(t);
    }

    Json::Value result;
    result["tasks"] = tasksJson;
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    callback(result_callback);
}