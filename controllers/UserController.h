#pragma once
#include <drogon/drogon.h>

class UserController : public drogon::HttpController<UserController>
{
  public:
    void registration_User(const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback);
    METHOD_LIST_BEGIN
      ADD_METHOD_TO(UserController::registration_User, "/registration", drogon::Post);
    METHOD_LIST_END
};
