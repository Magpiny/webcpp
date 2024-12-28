#include "ServicesPageCtrl.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>

// Add definition of your processing function here

void ServicesPageCtrl::getServicesPage(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) const
{

    // Add some dummy data
    HttpViewData data;
    data.insert("title", "Our Services");

    // Load Services Page UI
    auto res = HttpResponse::newHttpViewResponse("ServicesPage", data);

    callback(res);
};
