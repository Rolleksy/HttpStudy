#pragma once
#include "orm/model.hpp"
#include <sqlite3.h>
#include <sstream>
#include <stdexcept>

namespace orm {

template<typename T>
bool insert(sqlite3* db, const T& obj) {
    auto& info = ModelInfo<T>::fields;
    auto& tableName = ModelInfo<T>::tableName;
    std::ostringstream sql;
    
    sql << "INSERT INTO " << tableName << " (";
    
    // Add field names (skip id for auto-increment)
    bool first = true;
    for (size_t i = 0; i < info.size(); ++i) {
        if (info[i].name == "id") continue;
        if (!first) sql << ", ";
        sql << info[i].name;
        first = false;
    }
    
    sql << ") VALUES (";
    
    // Add placeholders
    first = true;
    for (size_t i = 0; i < info.size(); ++i) {
        if (info[i].name == "id") continue;
        if (!first) sql << ", ";
        sql << "?";
        first = false;
    }
    
    sql << ");";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    // Bind values (skip id)
    const char* objPtr = reinterpret_cast<const char*>(&obj);
    int paramIndex = 1;
    
    for (size_t i = 0; i < info.size(); ++i) {
        if (info[i].name == "id") continue;
        
        const auto& field = info[i];
        const char* fieldPtr = objPtr + field.offset;
        
        if (std::string(field.sqliteType) == "INTEGER") {
            int value = *reinterpret_cast<const int*>(fieldPtr);
            sqlite3_bind_int(stmt, paramIndex++, value);
        } else if (std::string(field.sqliteType) == "TEXT") {
            const std::string& value = *reinterpret_cast<const std::string*>(fieldPtr);
            sqlite3_bind_text(stmt, paramIndex++, value.c_str(), -1, SQLITE_TRANSIENT);
        }
    }
    
    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    
    return success;
}

}