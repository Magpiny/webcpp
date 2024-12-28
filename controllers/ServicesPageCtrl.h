#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class ServicesPageCtrl : public drogon::HttpController<ServicesPageCtrl> {
public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(ServicesPageCtrl::get, "/{2}/{1}", Get); // path is /ServicesPageCtrl/{arg2}/{arg1}
    // METHOD_ADD(ServicesPageCtrl::your_method_name, "/{1}/{2}/list", Get); // path is /ServicesPageCtrl/{arg1}/{arg2}/list
    ADD_METHOD_TO(ServicesPageCtrl::getServicesPage, "/services", Get); // path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    void getServicesPage(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;
};
