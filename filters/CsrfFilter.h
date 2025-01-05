/**
 *
 *  CsrfFilter.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;

class CsrfFilter : public HttpFilter<CsrfFilter> {
public:
    CsrfFilter() { }
    void doFilter(const HttpRequestPtr& req,
        FilterCallback&& fcb,
        FilterChainCallback&& fccb) override;

private:
    std::string generate_token();
    bool verify_token(const std::string& token);
};
