#include "ApiMessagesCtrl.h"
#include <algorithm>
#include <cmath>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/orm/Exception.h>
#include <functional>
#include <json/json.h>
#include <json/value.h>
#include <limits>
#include <string>

// Add definition of your processing function here
using namespace drogon;

void ApiMessagesCtrl::getMessages(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    int page = 1;
    int limit = 10;

    // Parse query parameters if they exist
    auto parameters = req->getParameters();
    try {
        if (parameters.find("page") != parameters.end()) {
            page = std::max(1, std::stoi(parameters["page"]));
        }
        if (parameters.find("limit") != parameters.end()) {
            limit = std::clamp(std::stoi(parameters["limit"]), min, max);
        }
    } catch (const std::exception& e) {
        Json::Value errorJson;
        errorJson["error"] = "Invalid parameter format";
        auto resp = HttpResponse::newHttpJsonResponse(errorJson);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    // Calculate offset
    int offset = (page - 1) * limit;

    // Copy of the callback
    auto cb_copy = callback;

    // Initialize the database
    auto dbClient = drogon::app().getDbClient();

    get_total_count([&, dbClient, page, limit, offset, cb_copy](int total_records) {
        int total_pages = (total_records + limit - 1) / limit;
        // Execute sql statement
        dbClient->execSqlAsync(get_messages_qry, [&, page, limit, total_pages, total_records, cb_copy](const drogon::orm::Result& result) {
        Json::Value responseJson(Json::arrayValue);
        Json::Value res;

        //convert the response to json
        for (const auto& row : result) {
            Json::Value messageObj;
            messageObj["id"] = row["id"].as<int>();
            messageObj["username"] = row["username"].as<std::string>();
            messageObj["email"] = row["email"].as<std::string>();
            messageObj["message"] = row["message"].as<std::string>();
            messageObj["created_at"] = row["created_at"].as<std::string>();

            responseJson.append(messageObj);
        }

        // Add pagination meta data to the respose
        res["data"] = responseJson;
        res["pagination"] = Json::Value();
        res["pagination"]["current_page"] = page;
        res["pagination"]["page_size"] = limit;
        res["pagination"]["total_pages"] = total_pages;
        res["pagination"]["total_records"] = total_records;
        res["pagination"]["has_next"] = static_cast<bool>(page<total_pages);
        res["pagination"]["has_previous"] = static_cast<bool>(page>1);


        auto resp = HttpResponse::newHttpJsonResponse(responseJson);
        resp->setStatusCode(drogon::k200OK);
        resp->addHeader("Access-Control-Allow-Origin", "*"); // for CORS

        cb_copy(resp); },

            [cb_copy](const orm::DrogonDbException& err) {
        Json::Value errorJson;
        errorJson["error"] = "Database Error Occured: "+std::string(err.base().what());
        auto resp = HttpResponse::newHttpJsonResponse(errorJson);
        resp->setStatusCode(k500InternalServerError);
        //
        cb_copy(resp); }, limit, offset);
    });
};

void ApiMessagesCtrl::get_total_count(const std::function<void(int)>& callback)
{
    auto dbClient = drogon::app().getDbClient();

    dbClient->execSqlAsync(records_total, [callback](const drogon::orm::Result& res) { 
        if (res.size()==0){
            callback(0);
            return;
        }
        callback(res[0]["total"].as<int>()); },
        //
        [callback](const drogon::orm::DrogonDbException& err) { callback(0); });
}
