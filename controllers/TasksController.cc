#include "TasksController.h"
#include "storage/MemoryStorage.h"

void TasksController::getTasks(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback
) {
    auto tasks = taskStorage->getAllTasks();

    Json::Value tasksJson(Json::arrayValue);

    for (auto &task : tasks) {
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

void TasksController::getTask(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    int taskId
) {
    auto task = taskStorage->getTaskById(taskId);

    if (!task.has_value()) {
        Json::Value result;
        result["error"] = "Task not found";
        auto result_callback =
            drogon::HttpResponse::newHttpJsonResponse(result);
        result_callback->setStatusCode(drogon::k404NotFound);
        callback(result_callback);
        return;
    }

    auto &task_res = task.value();
    Json::Value result;
    result["id"] = task_res.id;
    result["title"] = task_res.title;
    result["description"] = task_res.description;
    result["difficulty"] = task_res.difficulty;

    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    callback(result_callback);
}

void TasksController::createTask(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    auto jsonPtr = req->getJsonObject();
    if (!jsonPtr) {
        Json::Value ret;
        ret["error"] = "Invalid JSON";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    auto& json = *jsonPtr;

    if (!json.isMember("username")) {
        Json::Value ret;
        ret["error"] = "Missing username";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string username = json["username"].asString();

    MemoryStorage userStorage;
    std::string role = userStorage.getUserRole(username);
    if (role != "teacher") {
        Json::Value ret;
        ret["error"] = "Access denied. Only teachers can create tasks.";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k403Forbidden);
        callback(resp);
        return;
    }

    if (!json.isMember("title") || !json.isMember("description") || !json.isMember("difficulty")) {
        Json::Value ret;
        ret["error"] = "Missing fields: title, description, difficulty";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }
    std::string title = json["title"].asString();
    std::string description = json["description"].asString();
    std::string difficulty = json["difficulty"].asString();

    Task newTask;
    newTask.title = title;
    newTask.description = description;
    newTask.difficulty = difficulty;

    int taskId = taskStorage->addTask(newTask);
    if (taskId == -1) {
        Json::Value ret;
        ret["error"] = "Failed to create task";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
        return;
    }

    Json::Value ret;
    ret["status"] = "ok";
    ret["task_id"] = taskId;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}