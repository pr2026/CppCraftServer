#include "UserController.h"
#include <mutex>
#include <unordered_map>

void UserController::registration_User(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
  auto jsonPTR = req->getJsonObject();
  if (!jsonPTR) {
    Json::Value result;
    result["error"] = "Invalid Json";
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    result_callback->setStatusCode(drogon::k400BadRequest);
    callback(result_callback);
    return;
  }

  auto &json = *jsonPTR;
  if (!json.isMember("username") || !json.isMember("password")) {
    Json::Value result;
    result["error"] = "Invalid Json";
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    result_callback->setStatusCode(drogon::k400BadRequest);
    callback(result_callback);
    return;
  }

  std::string username = json["username"].asString();
  std::string password = json["password"].asString();

  if (storage_->userExists(username)) {
    Json::Value result;
    result["error"] = "Username already exists";
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    result_callback->setStatusCode(drogon::k409Conflict);
    callback(result_callback);
    return;
  }

  if (storage_->addUser(username, password)) {
    Json::Value result;
    result["status"] = "OK";
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    callback(result_callback);

  } else {
    Json::Value result;
    result["error"] = "Internal error";
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    result_callback->setStatusCode(drogon::k500InternalServerError);
    callback(result_callback);
  }
}

void UserController::login_User(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
  auto jsonPTR = req->getJsonObject();
  if (!jsonPTR) {
    Json::Value result;
    result["error"] = "Invalid Json";
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    result_callback->setStatusCode(drogon::k400BadRequest);
    callback(result_callback);
    return;
  }

  auto &json = *jsonPTR;
  if (!json.isMember("username") || !json.isMember("password")) {
    Json::Value result;
    result["error"] = "Invalid Json";
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    result_callback->setStatusCode(drogon::k400BadRequest);
    callback(result_callback);
    return;
  }

  std::string username = json["username"].asString();
  std::string password = json["password"].asString();

  if (storage_->checkPassword(username, password)){
    Json::Value result;
    result["status"] = "OK";
    result["role"] = "student";
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    callback(result_callback);
  }
  else {
    Json::Value result;
    result["error"] = "Invalid username or password";
    auto result_callback = drogon::HttpResponse::newHttpJsonResponse(result);
    result_callback->setStatusCode(drogon::k401Unauthorized);
    callback(result_callback);
    return;
  }
}