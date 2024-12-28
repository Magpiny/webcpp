#pragma once

#include <drogon/HttpController.h>
// #include <drogon/HttpResponse.h>
// #include <drogon/HttpTypes.h>

using namespace drogon;

class HomePageCtrl : public drogon::HttpController<HomePageCtrl> {
public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    ADD_METHOD_TO(HomePageCtrl::getHomePage, "/", drogon::Get);
    // METHOD_ADD(HomePageCtrl::get, "/{2}/{1}", Get); // path is /HomePageCtrl/{arg2}/{arg1}
    // METHOD_ADD(HomePageCtrl::your_method_name, "/{1}/{2}/list", Get); // path is /HomePageCtrl/{arg1}/{arg2}/list
    // ADD_METHOD_TO(HomePageCtrl::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void getHomePage(const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback) const;
};
