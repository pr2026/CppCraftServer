#pragma once
#include <drogon/drogon.h>
#include "storage/MemoryStorage.h"
#include <memory>

class UserController : public drogon::HttpController<UserController>
{
  public:
    UserController() : storage_(std::make_unique<MemoryStorage>()) {};


    void registration_User(const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback);
    void login_User(const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback);
    METHOD_LIST_BEGIN
      ADD_METHOD_TO(UserController::registration_User, "/registration", drogon::Post);
      ADD_METHOD_TO(UserController::login_User, "/login", drogon::Post);
    METHOD_LIST_END
  
    private:
      std::unique_ptr<Storage> storage_;
};