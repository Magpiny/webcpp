#pragma once

#include <drogon/HttpController.h>
#include <drogon/HttpTypes.h>

using namespace drogon;

class AboutPageCtrl : public drogon::HttpController<AboutPageCtrl> {
public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(AboutPageCtrl::get, "/{2}/{1}", Get); // path is /AboutPageCtrl/{arg2}/{arg1}
    ADD_METHOD_TO(AboutPageCtrl::getAboutPage, "/about", drogon::Get);
    // METHOD_ADD(AboutPageCtrl::your_method_name, "/{1}/{2}/list", Get); // path is /AboutPageCtrl/{arg1}/{arg2}/list
    // ADD_METHOD_TO(AboutPageCtrl::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    void getAboutPage(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;
};
