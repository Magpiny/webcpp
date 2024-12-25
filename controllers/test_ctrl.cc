#include "test_ctrl.h"
#include <drogon/HttpResponse.h>

void test_ctrl::asyncHandleHttpRequest(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{
    // write your application logic here

    auto res = HttpResponse::newHttpResponse(k200OK, CT_TEXT_HTML);
    res->setBody("Hello from controller");
    callback(res);
}
