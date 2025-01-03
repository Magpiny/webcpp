#pragma once

#include <argon2.h>
#include <cstdint>
#include <drogon/HttpController.h>

using namespace drogon;

class ContactsPageCtrl : public drogon::HttpController<ContactsPageCtrl> {
public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(ContactsPageCtrl::getContactsPage, "/contact", Get); // path is /absolute/path/{arg1}/{arg2}/list
    ADD_METHOD_TO(ContactsPageCtrl::postMessage, "/contact", Post);

    METHOD_LIST_END

    void postSth();
    void getContactsPage(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    void postMessage(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback); // Handle contact form submission

private:
    const std::int_fast16_t valid_email_length = 254;
    bool validateEmail(std::string& email);

    // QUeries
    const std::string insert_contact_qry = "INSERT INTO contacts (username, email, message) VALUES ($1, $2, $3)";
};
