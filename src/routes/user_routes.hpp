#pragma once
#include "router.hpp"
#include "db.hpp"
#include "models/user.hpp"
#include "orm/crud.hpp"
#include "orm/schema.hpp"
#include <sstream>
#include <iostream>

inline std::string userToJson(const User& user) {
    std::ostringstream json;
    json << "{"
         << "\"id\":" << user.id << ","
         << "\"name\":\"" << user.name << "\","
         << "\"age\":" << user.age
         << "}";
    return json.str();
}

inline std::string usersToJson(const std::vector<User>& users) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < users.size(); ++i) {
        json << userToJson(users[i]);
        if (i != users.size() - 1) json << ",";
    }
    json << "]";
    return json.str();
}

inline User parseUserFromBody(const std::string& body) {
    User user;
    // Simple parsing - in production use a JSON library
    // Format expected: name=John&age=25
    
    size_t namePos = body.find("name=");
    size_t agePos = body.find("age=");
    
    if (namePos != std::string::npos) {
        size_t start = namePos + 5;
        size_t end = body.find("&", start);
        if (end == std::string::npos) end = body.length();
        user.name = body.substr(start, end - start);
    }
    
    if (agePos != std::string::npos) {
        size_t start = agePos + 4;
        size_t end = body.find("&", start);
        if (end == std::string::npos) end = body.length();
        user.age = std::stoi(body.substr(start, end - start));
    }
    
    return user;
}

inline void register_user_routes() {
    auto db = DB::getInstance().get();
    
    // Create table if not exists
    orm::createTable<User>(db);
    
    std::cerr << "[INIT] Registering user routes..." << std::endl;
    
    // GET all users
    Router::get("/api/users", [](auto req) {
        auto db = DB::getInstance().get();
        auto users = orm::getAll<User>(db);
        return HttpResponse{200, usersToJson(users), "application/json"};
    });
    std::cerr << "[INIT] Registered GET /api/users" << std::endl;
    
    // POST create user
    Router::post("/api/users", [](auto req) {
        auto db = DB::getInstance().get();
        User user = parseUserFromBody(req.body);
        user.id = 0; // SQLite will auto-increment
        
        if (orm::insert(db, user)) {
            return HttpResponse{201, "{\"message\":\"User created\"}", "application/json"};
        }
        return HttpResponse{500, "{\"error\":\"Failed to create user\"}", "application/json"};
    });
    std::cerr << "[INIT] Registered POST /api/users" << std::endl;

    // PUT update user
    Router::put("/api/users/{id}", [](auto req) {
        auto db = DB::getInstance().get();
        // Extract id from path
        size_t idStart = req.path.find_last_of('/') + 1;
        int id = std::stoi(req.path.substr(idStart));
        User user = parseUserFromBody(req.body);
        user.id = id;

        if (orm::update(db, id, user)) {
            return HttpResponse{200, "{\"message\":\"User updated\"}", "application/json"};
        }
        return HttpResponse{500, "{\"error\":\"Failed to update user\"}", "application/json"};
    });
    std::cerr << "[INIT] Registered PUT /api/users/{id}" << std::endl;


    // DELETE user
    Router::del("/api/users/{id}", [](auto req) {
        auto db = DB::getInstance().get();
        // Extract id from path
        size_t idStart = req.path.find_last_of('/') + 1;
        int id = std::stoi(req.path.substr(idStart));

        if (orm::remove<User>(db, id)) {
            return HttpResponse{200, "{\"message\":\"User deleted\"}", "application/json"};
        }
        return HttpResponse{500, "{\"error\":\"Failed to delete user\"}", "application/json"};
    });
    std::cerr << "[INIT] Registered DELETE /api/users/{id}" << std::endl;
}