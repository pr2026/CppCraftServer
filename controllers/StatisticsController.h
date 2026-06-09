#pragma once
#include <drogon/HttpController.h>
#include "StatisticsDB.h"

class StatisticsController : public drogon::HttpController<StatisticsController> {
public:
    void getUserStatistics(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback, int userId);

    METHOD_LIST_BEGIN
    ADD_METHOD_TO(StatisticsController::getUserStatistics, "/students/{user_id}/statistics", drogon::Get);
    METHOD_LIST_END
};