#include "AuthCtrl.h"

#include <drogon/orm/Mapper.h>
#include <string>

// DISPLAY login page
void AuthCtrl::showLogin(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto session = req->getSession();
    if (session && session->find("user_id")) {
        auto resp = HttpResponse::newRedirectionResponse("/");
        callback(resp);
        return;
    }

    HttpViewData data;
    data.insert("title", "Welcome back..");
    auto resp = HttpResponse::newHttpViewResponse("login", data);
    callback(resp);
};

// DISPLAY registration page
void AuthCtrl::showRegister(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto session = req->getSession();
    if (session && session->find("user_id")) {
        auto resp = HttpResponse::newRedirectionResponse("/");
        callback(resp);
        return;
    }

    HttpViewData data;
    data.insert("title", "Create Account");
    auto resp = HttpResponse::newHttpViewResponse("registration", data);
    callback(resp);
}

void AuthCtrl::logout(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto session = req->getSession();
    if (session) {
        session->erase("user_id");
    }
    auto resp = HttpResponse::newRedirectionResponse("/login");
    callback(resp);
}

// Process login

void AuthCtrl::processLogin(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) { };

// Handle registration processing

void AuthCtrl::processRegister(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) { };
