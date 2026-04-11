#include <drogon/drogon.h>
#include "controllers/TasksController.h"
#include "controllers/UserController.h"

int main() {
    drogon::app()
        .setLogLevel(trantor::Logger::kTrace)
        .addListener("0.0.0.0", 8080)
        .run();
}
