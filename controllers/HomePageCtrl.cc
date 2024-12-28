#include "HomePageCtrl.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>
#include <trantor/utils/Logger.h>

// Add definition of your processing function here
void HomePageCtrl::getHomePage(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) const
{
    LOG_INFO << "HomePageCtrl controller called";
    // insert some data
    HttpViewData data;
    data.insert("title", "Drogon is Awesome");
    data.insert("message", "The Dragon");

    // Render the template
    auto resp = HttpResponse::newHttpViewResponse("HomePage", data);

    // Add security headers
    resp->addHeader("X-Frame-Options", "DENY");
    resp->addHeader("X-Content-Type-Options", "nosniff");
    resp->addHeader("X-XSS-Protection", "1; mode=block");
    resp->addHeader("Referrer-Policy", "strict-origin-when-cross-origin");

    callback(resp);
}
