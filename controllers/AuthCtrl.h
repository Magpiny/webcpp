#pragma once

#include <argon2.h>
#include <drogon/HttpController.h>
#include <random>

using namespace drogon;

class AuthCtrl : public drogon::HttpController<AuthCtrl> {
public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(AuthCtrl::showLogin, "/login", Get);
    ADD_METHOD_TO(AuthCtrl::showRegister, "/register", Get);

    ADD_METHOD_TO(AuthCtrl::processLogin, "/login", Post);
    ADD_METHOD_TO(AuthCtrl::processRegister, "/register", Post);
    ADD_METHOD_TO(AuthCtrl::logout, "/logout", Post);

    METHOD_LIST_END

    void showLogin(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    void showRegister(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    void processLogin(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    void processRegister(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    void logout(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
};

// Password Hasher

class PasswordHasher {
private:
    static constexpr uint32_t SALT_LENGTH = 16;
    static constexpr uint32_t HASH_LENGTH = 32;
    static constexpr uint32_t TIME_COST = 2;
    static constexpr uint32_t MEMORY_COST = 1 << 16; // 64 MiB
    static constexpr uint32_t PARALLELISM = 4;

    static std::string generateSalt()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        std::string salt;
        salt.reserve(SALT_LENGTH);
        for (size_t i = 0; i < SALT_LENGTH; ++i) {
            salt.push_back(static_cast<char>(dis(gen)));
        }
        return salt;
    }

public:
    static std::string hashPassword(const std::string& password)
    {
        std::string salt = generateSalt();
        uint8_t hash[HASH_LENGTH];

        argon2id_hash_raw(
            TIME_COST,
            MEMORY_COST,
            PARALLELISM,
            password.c_str(),
            password.length(),
            salt.c_str(),
            SALT_LENGTH,
            hash,
            HASH_LENGTH);

        // Combine salt and hash for storage
        std::string result;
        result.reserve(SALT_LENGTH + HASH_LENGTH);
        result.append(salt);
        result.append(reinterpret_cast<char*>(hash), HASH_LENGTH);

        return result;
    }

    static bool verifyPassword(const std::string& password, const std::string& stored)
    {
        if (stored.length() != SALT_LENGTH + HASH_LENGTH) {
            return false;
        }

        std::string salt = stored.substr(0, SALT_LENGTH);
        std::string storedHash = stored.substr(SALT_LENGTH);

        uint8_t computedHash[HASH_LENGTH];
        argon2id_hash_raw(
            TIME_COST,
            MEMORY_COST,
            PARALLELISM,
            password.c_str(),
            password.length(),
            salt.c_str(),
            SALT_LENGTH,
            computedHash,
            HASH_LENGTH);

        return std::memcmp(computedHash, storedHash.c_str(), HASH_LENGTH) == 0;
    }
};
