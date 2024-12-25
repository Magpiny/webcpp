#include <drogon/drogon.h>
#include <trantor/utils/Logger.h>

using namespace drogon;

int main()
{
    const int PORT = 5555;
    // Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", PORT);
    // Load config file
    drogon::app().loadConfigFile("../config.json");
    // drogon::app().loadConfigFile("../config.yaml");
    // Run HTTP framework,the method will block in the internal event loop

    drogon::HttpAppFramework::instance()
        .registerHandler("/list_para",
            [=](const HttpRequestPtr& req,
                std::function<void(const HttpResponsePtr&)>&& callback) {
                auto para = req->getParameters();
                HttpViewData data;
                data.insert("title", "ListParameters");
                data.insert("parameters", para);
                auto resp = HttpResponse::newHttpViewResponse("ListParams.csp", data);
                callback(resp);
            });

    LOG_INFO << "Drogon Web server running on PORT: " << PORT << "\n";

    drogon::app().run();
    return 0;
}
