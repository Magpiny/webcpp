#include "ContactsPageCtrl.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>

// Add definition of your processing function here

void ContactsPageCtrl::getContactsPage(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) const
{

    // Add some dummy data
    HttpViewData data;
    data.insert("title", "Contact Us");

    // Load About
    auto res = HttpResponse::newHttpViewResponse("ContactsPage", data);

    callback(res);
};

void ContactsPageCtrl::postContact(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) const
{
    // Handle form submission here
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    callback(resp);
}
