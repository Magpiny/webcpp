#pragma once

#include <drogon/CacheMap.h>
#include <drogon/HttpController.h>
#include <drogon/RateLimiter.h>
#include <string>

using namespace drogon;

class ApiMessagesCtrl : public drogon::HttpController<ApiMessagesCtrl> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ApiMessagesCtrl::getMessages, "/api/messages", Get);
    METHOD_LIST_END

    // Initialize cache with 1hr expiration time 15:28:22
    // ApiMessagesCtrl()
    //{
    // std::shared_ptr<drogon::CacheMap<std::string, Json::Value>> messagesCache;
    // messagesCache = std::make_shared<drogon::CacheMap<std::string, Json::Value>>(3600);

    // const std::string cache_key = "messages_all";
    //}

    void getMessages(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

    static HttpResponsePtr createErrorResponse(const std::string& message,
        HttpStatusCode code);

private:
    // Minimum and Maximum limit range for pagination
    const int min = 1;
    const int max = 100;

    const std::string get_messages_qry
        = "SELECT id, username, email, message, created_at FROM contacts ORDER BY created_at DESC LIMIT $1::integer OFFSET $2::integer";
    // get the number of records
    const std::string records_total = "SELECT COUNT(*) AS total FROM contacts";

    // Helper method to get total count of messages
    void get_total_count(const std::function<void(int)>& callback);
};
