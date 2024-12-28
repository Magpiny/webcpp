#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class ContactsPageCtrl : public drogon::HttpController<ContactsPageCtrl> {
public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(ContactsPageCtrl::get, "/{2}/{1}", Get); // path is /ContactsPageCtrl/{arg2}/{arg1}
    // METHOD_ADD(ContactsPageCtrl::your_method_name, "/{1}/{2}/list", Get); // path is /ContactsPageCtrl/{arg1}/{arg2}/list
    ADD_METHOD_TO(ContactsPageCtrl::getContactsPage, "/contact", Get); // path is /absolute/path/{arg1}/{arg2}/list
    ADD_METHOD_TO(ContactsPageCtrl::postContact, "/contact", Post);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    void getContactsPage(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;
    void postContact(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback) const;
};
