#pragma once

#include <cstdint>
#include <drogon/HttpController.h>
#include <string>

using namespace drogon;

class ApiMessagesCtrl : public drogon::HttpController<ApiMessagesCtrl> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ApiMessagesCtrl::getMessages, "/api/messages", Get);
    METHOD_LIST_END

    void getMessages(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

private:
    // Minimum and Maximum limit range
    const int min = 1;
    const int max = 100;

    const std::string get_messages_qry
        = "SELECT id, username, email, message, created_at FROM contacts ORDER BY created_at DESC LIMIT $1::integer OFFSET $2::integer";
    // get the number of records
    const std::string records_total = "SELECT COUNT(*) AS total FROM contacts";

    // Helper method to get total count of messages
    void get_total_count(const std::function<void(int)>& callback);
};
