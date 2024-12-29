#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class ContactsPageCtrl : public drogon::HttpController<ContactsPageCtrl> {
public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(ContactsPageCtrl::getContactsPage, "/contact", Get); // path is /absolute/path/{arg1}/{arg2}/list
    ADD_METHOD_TO(ContactsPageCtrl::postMessage, "/contact", Post);

    METHOD_LIST_END

    void getContactsPage(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    void postMessage(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback); // Handle contact form submission

private:
    bool validateEmail(std::string& email);
};
