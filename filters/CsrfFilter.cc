/**
 *
 *  CsrfFilter.cc
 *
 */

#include "CsrfFilter.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/HttpViewData.h>
#include <drogon/utils/Utilities.h>

using namespace drogon;

std::string CsrfFilter::generate_token()
{
    return drogon::utils::getUuid();
};

bool CsrfFilter::verify_token(const std::string& token)
{
    return !token.empty();
};

void CsrfFilter::doFilter(const HttpRequestPtr& req,
    FilterCallback&& fcb,
    FilterChainCallback&& fccb)
{
    HttpViewData data;
    std::string token = generate_token();

    auto session = req->session();
    auto session_token = session->get<std::string>("csrf_token");

    if (!session->find("csrf_token")) {
        session->insert("csrf_token", token);
        data.insert("csrf_token", token);

        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k201Created);
        resp->addHeader("X-CSRF-Token", token);
        fcb(resp);
    }

    // Skip CSRF check for GET, HEAD OPTIONS requests
    if (req->method() == Get || req->method() == Head || req->method() == drogon::Options) {

        // For GET requests, set CSRF token if it doesn't exit
        if (!session->find("csrf_token")) {
            session->insert("csrf_token", token);
            data.insert("csrf_token", token);
        }
        fccb();
        return;
    }

    // Check token from request header
    auto header_token = req->getHeader("X-CSRF-Token");
    if (header_token.empty()) {
        header_token = req->getParameter("_csrf");
    }

    if (session_token.empty() || header_token.empty()) {
        // No token in session, this is an error state
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k403Forbidden);
        resp->setBody("CSRF token missing in session");
        fcb(resp);
        return;
    }

    if (!verify_token(header_token) || header_token != session_token) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k403Forbidden);
        resp->setBody("CSRF token validation failed");
        fcb(resp);
        return;
    }

    // If token is valid continue with the request;
    fccb();
}
