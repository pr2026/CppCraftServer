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
    auto taskOpt = taskStorage->getTaskById(taskId);
    if (!taskOpt.has_value()) {
        Json::Value ret;
        ret["error"] = "Task not found";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k404NotFound);
        callback(resp);
        return;
    }
    const auto& task = taskOpt.value();
    Json::Value ret;
    ret["id"] = task.id;
    ret["title"] = task.title;
    ret["description"] = task.description;
    ret["difficulty"] = task.difficulty;

    Json::Value testsArray(Json::arrayValue);
    for (const auto& test : task.tests) {
        Json::Value testJson;
        testJson["input"] = test.input;
        testJson["expected_output"] = test.expected_output;

        testsArray.append(testJson);
    }
    ret["tests"] = testsArray;

    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
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

    if (json.isMember("tests") && json["tests"].isArray()) {
        for (const auto& test : json["tests"]) {
            if (test.isMember("input") && test.isMember("expected_output")) {
                std::string input = test["input"].asString();
                std::string expected = test["expected_output"].asString();
                taskStorage->addTest(taskId, input, expected);
            }
        }
    }

    auto taskOpt = taskStorage->getTaskById(taskId);
    if (!taskOpt.has_value()) {
        Json::Value ret;
        ret["error"] = "Task created but not found";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
        return;
    }
    const auto& task = taskOpt.value();

    Json::Value ret;
    ret["status"] = "ok";
    ret["task_id"] = taskId;
    ret["title"] = task.title;
    ret["description"] = task.description;
    ret["difficulty"] = task.difficulty;

    Json::Value testsArray(Json::arrayValue);
    for (const auto& test : task.tests) {
        Json::Value t;
        t["input"] = test.input;
        t["expected_output"] = test.expected_output;
        testsArray.append(t);
    }
    ret["tests"] = testsArray;

    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}


void TasksController::deleteTask(const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        int taskId){
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
        ret["error"] = "Access denied. Only teachers can delete tasks.";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k403Forbidden);
        callback(resp);
        return;
    }

    bool deleted = taskStorage->deleteTask(taskId);
    if (!deleted){
        Json::Value ret;
        ret["error"] = "Task not found";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k404NotFound);
        callback(resp);
        return;
    }

    Json::Value ret;
    ret["status"] = "ok";
    ret["task_id"] = taskId;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void TasksController::updateTask(const drogon::HttpRequestPtr& req,
                                 std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                                 int taskId) {
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
    auto userOpt = userStorage.getUserByUsername(username);
    if (!userOpt.has_value()) {
        Json::Value ret;
        ret["error"] = "User not found";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k404NotFound);
        callback(resp);
        return;
    }
    if (userOpt->role != "teacher") {
        Json::Value ret;
        ret["error"] = "Access denied. Only teachers can update tasks";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k403Forbidden);
        callback(resp);
        return;
    }
    int currentUserId = userOpt->id;

    auto taskOpt = taskStorage->getTaskById(taskId);
    if (!taskOpt.has_value()) {
        Json::Value ret;
        ret["error"] = "Task not found";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k404NotFound);
        callback(resp);
        return;
    }
    const auto& existingTask = taskOpt.value();
    if (existingTask.ownerId != currentUserId) {
        Json::Value ret;
        ret["error"] = "Access denied. You are not the owner of this task";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k403Forbidden);
        callback(resp);
        return;
    }

    if (!json.isMember("title") || !json.isMember("description") || !json.isMember("difficulty")) {
        Json::Value ret;
        ret["error"] = "Missing required fields: title, description, difficulty";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    Task updatedTask;
    updatedTask.id = taskId;
    updatedTask.title = json["title"].asString();
    updatedTask.description = json["description"].asString();
    updatedTask.difficulty = json["difficulty"].asString();
    updatedTask.ownerId = currentUserId;

    if (json.isMember("tests") && json["tests"].isArray()) {
        updatedTask.tests.clear();
        for (const auto& test : json["tests"]) {
            if (test.isMember("input") && test.isMember("expected_output")) {
                TestCase tc;
                tc.input = test["input"].asString();
                tc.expected_output = test["expected_output"].asString();
                updatedTask.tests.push_back(tc);
            }
        }
    } 
    else updatedTask.tests = existingTask.tests;

    if (!taskStorage->updateTask(taskId, updatedTask)) {
        Json::Value ret;
        ret["error"] = "Failed to update task";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
        return;
    }

    Json::Value ret;
    ret["status"] = "OK";
    ret["task_id"] = taskId;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}