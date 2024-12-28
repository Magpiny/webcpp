#include <drogon/drogon.h>
#include <memory>
#include <trantor/utils/Logger.h>

using namespace drogon;
#include "controllers/HomePageCtrl.h"

int main()
{
    const int PORT = 5555;
    const std::shared_ptr<HomePageCtrl> controller = std::make_shared<HomePageCtrl>();

    // Load config file
    drogon::app().loadConfigFile("./config.json");
    // drogon::app().registerController(controller);

    LOG_INFO << "Drogon Web server running on PORT: " << PORT << "\n";

    drogon::app().run();

    return 0;
}
