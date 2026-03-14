#include <drogon/drogon.h>
#include "controllers/UserController.h"
#include "controllers/TasksController.h"

int main() {
    drogon::app().setLogLevel(trantor::Logger::kTrace)
        .addListener("0.0.0.0",8080)
        .run();
}
