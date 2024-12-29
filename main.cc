#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <trantor/utils/Logger.h>

using namespace drogon;

int main()
{
    // Load config file
    drogon::app().loadConfigFile("./config.json");

    LOG_INFO << "Drogon Web server running on PORT: 5555" << "\n";

    drogon::app().run();

    return 0;
}
