/**
 *
 *  CsrfFilter.cc
 *
 */

#include "CsrfFilter.h"

using namespace drogon;

void CsrfFilter::doFilter(const HttpRequestPtr& req,
    FilterCallback&& fcb,
    FilterChainCallback&& fccb)
{

    if (req->method() != Get) {
        auto csrfCookie = req->getCookie("_csrf");
        auto csrfToken = req->getParameter("_csrf");

        if (csrfCookie.empty() || csrfToken.empty() || csrfCookie != csrfToken) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k403Forbidden);
            resp->setBody("CSRF validation failed");
            fcb(resp);
            return;
        }
    }
    fccb();
}
