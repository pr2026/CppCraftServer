#include "UserController.h"
#include <mutex>
#include <unordered_map>

static std::unordered_map<std::string, std::string> users;
static std::mutex users_Mutex;

void UserController::registration_User(const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback)
{
    auto jsonPTR = req->getJsonObject();
    if (!jsonPTR){
        Json::Value result;
        result["error"] = "Invalid Json";
        auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
        result_callback->setStatusCode(drogon::k400BadRequest);
        callback(result_callback);
        return;
    }

    auto &json = *jsonPTR;
    if (!json.isMember("username") || !json.isMember("password")){
        Json::Value result;
        result["error"] = "Invalid Json";
        auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
        result_callback->setStatusCode(drogon::k400BadRequest);
        callback(result_callback);
        return;
    }

    std::string username = json["username"].asString();
    std::string password = json["password"].asString();

    {
        std::lock_guard<std::mutex> lock(users_Mutex);
        if (users.find(username) != users.end()){
            Json::Value result;
            result["error"] = "Invalid Json";
            auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
            result_callback->setStatusCode(drogon::k400BadRequest);
            callback(result_callback);
            return;
        }
    }

    {
        std::lock_guard<std::mutex> lock(users_Mutex);
        users[username] = password;
    }

    Json::Value result;
    result["status"] = "OK";
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    callback(result_callback);
}