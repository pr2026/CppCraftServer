#pragma once
#include <drogon/drogon.h>
#include <memory>
#include "UserDB.h"

class UserController : public drogon::HttpController<UserController> {
public:
    UserController() : storage(std::make_unique<UserDB>("cppcraft.db")){};

    void registrationUser(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback
    );
    void loginUser(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback
    );
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(
        UserController::registrationUser,"/registration", drogon::Post
    );
    ADD_METHOD_TO(UserController::loginUser, "/login", drogon::Post);
    METHOD_LIST_END

private:
    std::unique_ptr<UserDB> storage;
};