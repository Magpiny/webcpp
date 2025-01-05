#include "AuthCtrl.h"

#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/HttpViewData.h>
#include <drogon/orm/Mapper.h>
#include <regex>
#include <string>
// Server side email verification: just incase the user has js disabled in the browser

bool AuthCtrl::is_email_valid(const std::string& email)
{

    const std::regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return std::regex_match(email, pattern);
};

// Helper function for Adding security headers
void addSecurityHeaders(const HttpResponsePtr& resp)
{
    resp->addHeader("X-Content-Type-Options", "nosniff");
    resp->addHeader("X-Frame-Options", "DENY");
    resp->addHeader("X-XSS-Protection", "1; mode=block");
    resp->addHeader("Content-Security-Policy", "default-src 'self'");
    resp->addHeader("Referrer-Policy", "strict-origin-when-cross-origin");
}

// Helper function for Adding cookie token
void setCsrfCookie(const HttpResponsePtr& resp)
{
    auto token = drogon::utils::getUuid();
    resp->addCookie("_csrf", token);
    resp->addHeader("X-CSRF-Token", token);
}

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
    data.insert("csrf_token", session->get<std::string>("csrf_token"));

    auto resp = HttpResponse::newHttpViewResponse("login", data);
    resp->addHeader("X-CSRF-Token", session->get<std::string>("csrf_token"));

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
    data.insert("csrf_token", session->get<std::string>("csrf_token"));

    auto resp = HttpResponse::newHttpViewResponse("registration", data);
    resp->addHeader("X-CSRF-Token", session->get<std::string>("csrf_token"));

    callback(resp);
}

void AuthCtrl::logout(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback)
{

    auto session = req->getSession();
    if (session) {
        session->erase("user_id");
    }
    Json::Value ret;
    ret["status"] = "success";
    ret["message"] = "Logged out successfully";
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

// Process login

void AuthCtrl::processLogin(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto params = req->getParameters();

    std::string username = params["username"];
    std::string password = params["password"];

    HttpViewData mssg_error;
    HttpViewData mssg_success;

    if (username.empty() || password.empty()) {
        Json::Value err_resp;
        err_resp["status"] = "error";
        err_resp["message"] = "All fields are required";

        mssg_error.insert("error", "All fields are required");
        auto resp = HttpResponse::newHttpViewResponse("/login", mssg_error);

        // auto resp = HttpResponse::newHttpJsonResponse(err_resp);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }
    try {
        auto clientPtr = app().getDbClient();
        auto result = clientPtr->execSqlSync(
            "SELECT id, password_hash FROM users WHERE username=$1",
            username);

        if (result.size() == 0) {
            Json::Value ret;
            ret["status"] = "error";
            ret["message"] = "Invalid credentials";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(k401Unauthorized);
            callback(resp);
            return;
        }

        std::string stored_hash = drogon::utils::base64Decode(result[0]["password_hash"].as<std::string>());

        // Split salt and hash
        std::string salt = stored_hash.substr(0, 16);
        std::string hash = stored_hash.substr(16);

        uint8_t computed_hash[32];
        if (argon2id_hash_raw(2, 1 << 16, 4, password.c_str(), password.length(),
                reinterpret_cast<const uint8_t*>(salt.c_str()), 16,
                computed_hash, sizeof(computed_hash))
            != ARGON2_OK) {
            Json::Value ret;
            ret["status"] = "error";
            ret["message"] = "Authentication failed";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(k500InternalServerError);
            callback(resp);
            return;
        }

        if (std::memcmp(computed_hash, hash.c_str(), 32) == 0) {
            auto session = req->getSession();
            session->insert("user_id", result[0]["id"].as<int>());

            if (req->getHeader("Accept").find("application/json") != std::string::npos) {
                // ... existing JSON success response ...
                Json::Value ret;
                ret["status"] = "success";
                ret["message"] = "Login successful";
                auto resp = HttpResponse::newHttpJsonResponse(ret);
                callback(resp);
            } else {
                auto resp = HttpResponse::newRedirectionResponse("/");
                addSecurityHeaders(resp);
                resp->addCookie("flash_message", "Welcome back!");
                resp->addCookie("flash_type", "success");
                callback(resp);
            }
        } else {
            if (req->getHeader("Accept").find("application/json") != std::string::npos) {

                Json::Value ret;
                ret["status"] = "error";
                ret["message"] = "Invalid credentials";
                auto resp = HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(k401Unauthorized);
                callback(resp);
            } else {
                auto resp = HttpResponse::newRedirectionResponse("/login");
                addSecurityHeaders(resp);
                resp->addCookie("flash_message", "Invalid credentials");
                resp->addCookie("flash_type", "error");
                callback(resp);
            }
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        LOG_ERROR << "Database error: " << e.base().what();

        if (req->getHeader("Accept").find("application/json") != std::string::npos) {

            Json::Value ret;
            ret["status"] = "error";
            ret["message"] = "Server error";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(k500InternalServerError);
            callback(resp);
        } else {
            auto resp = HttpResponse::newRedirectionResponse("/login");
            addSecurityHeaders(resp);
            resp->addCookie("flash_message", "Internal Server Error");
            resp->addCookie("flash_type", "error");
            callback(resp);
        }
    }
};

// Handle registration processing

void AuthCtrl::processRegister(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto params = req->getParameters();

    // Extract form values from the url parameters on POST
    std::string username = params["username"];
    std::string email = params["email"];
    std::string password = params["password"];

    // Return error on empty form fields
    if (username.empty() || email.empty() || password.empty()) {
        Json::Value mssg_err;
        mssg_err["status"] = "error";
        mssg_err["message"] = "Missing required fields";
        auto resp = HttpResponse::newHttpJsonResponse(mssg_err);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    if (!is_email_valid(email)) {
        Json::Value svr_resp;
        svr_resp["status"] = "error";
        svr_resp["message"] = "Invalid email address";
        auto resp = HttpResponse::newHttpJsonResponse(svr_resp);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    // Hash password with Argon2
    uint8_t hash[32];
    uint8_t salt[16];

    // Generate random salt
    std::random_device r_dev;
    std::mt19937 gen(r_dev());
    std::uniform_int_distribution<> dis(0, 255);
    for (int i = 0; i < 16; i++) {
        salt[i] = dis(gen);
    }

    if (argon2id_hash_raw(2, 1 << 16, 4, password.c_str(), password.length(),
            salt, sizeof(salt), hash, sizeof(hash))
        != ARGON2_OK) {
        Json::Value ret;
        ret["status"] = "error";
        ret["message"] = "Password hashing failed";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
        return;
    }

    // Convert to base64
    std::string password_hash = drogon::utils::base64Encode(
        std::string(reinterpret_cast<char*>(salt), 16) + std::string(reinterpret_cast<char*>(hash), 32));

    try {
        auto clientPtr = app().getDbClient();
        clientPtr->execSqlSync(
            "INSERT INTO users (username, email, password_hash) VALUES ($1, $2, $3)",
            username,
            email,
            password_hash);

        // Check if request expects JSON
        if (req->getHeader("Accept").find("application/json") != std::string::npos) {
            Json::Value ret;
            ret["status"] = "success";
            ret["message"] = "Registration successful";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            addSecurityHeaders(resp);
            callback(resp);
        } else {
            // Redirect to login with success message
            auto resp = HttpResponse::newRedirectionResponse("/login");
            addSecurityHeaders(resp);
            resp->addCookie("flash_message", "Registration successful! Please login.");
            resp->addCookie("flash_type", "success");
            callback(resp);
        }
    } catch (const drogon::orm::DrogonDbException& e) {
        LOG_ERROR << "Database error: " << e.base().what();

        if (req->getHeader("Accept").find("application/json") != std::string::npos) {
            Json::Value ret;
            ret["status"] = "error";
            ret["message"] = "Username or email already exists";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(k400BadRequest);
            callback(resp);
        } else {
            auto resp = HttpResponse::newRedirectionResponse("/register");
            addSecurityHeaders(resp);
            resp->addCookie("flash_message", "Username or email already exists");
            resp->addCookie("flash_type", "error");
            callback(resp);
        }
    }
};
