#include "AboutPageCtrl.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>

// Add definition of your processing function here

void AboutPageCtrl::getAboutPage(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) const
{

    // Add some dummy data
    HttpViewData data;
    data.insert("title", "About Us");

    // Load About
    auto res = HttpResponse::newHttpViewResponse("AboutPage", data);

    callback(res);
};
