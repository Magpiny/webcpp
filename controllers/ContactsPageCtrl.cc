#include "ContactsPageCtrl.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>
#include <regex>
#include <string>

// Server side email validation
bool ContactsPageCtrl::validateEmail(std::string& email)
{
    const std::regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return std::regex_match(email, pattern);
}

// Display Contacts Page UI
void ContactsPageCtrl::getContactsPage(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{

    // Add some dummy data
    HttpViewData data;
    data.insert("title", "Contact Us");
    data.insert("req", req);

    // Load Contacts Page
    auto res = HttpResponse::newHttpViewResponse("ContactsPage", data);

    callback(res);
};

// Handle ContactsPage form submission
void ContactsPageCtrl::postMessage(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    // Capture user data from the form
    auto params = req->getParameters();

    std::string username = params["username"];
    std::string email = params["email"];
    std::string message = params["message"];

    // Validate user input: serverside validation
    if (username.empty() || email.empty() || message.empty()) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("All fields are required");
        callback(resp);
        return;
    };

    // Validate user email
    if (!validateEmail(email)) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Invalid email format");
        callback(resp);
        return;
    };

    // Execute Query and Store data in a database (postgress database)
    auto client_ptr = app().getDbClient();
    const std::string insert_contact_qry = "INSERT INTO contacts (username, email, message) VALUES ($1, $2, $3)";

    client_ptr->execSqlAsync(
        insert_contact_qry,
        [callback](const drogon::orm::Result& res) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k302Found);
            resp->addHeader("Location", "/contact?status=success");
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException& err) {
            LOG_ERROR << "Database error: " << err.base().what();
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k500InternalServerError);
            resp->setBody("Failed to save contact. Please try again.");
            callback(resp);
        },
        username, email, message);
}
