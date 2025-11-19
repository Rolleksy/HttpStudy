#pragma once
#include "orm/model.hpp"
#include <sqlite3.h>
#include <sstream>

namespace orm {

template<typename T>
bool update(sqlite3* db, int id, const T& obj) {
    auto& info = ModelInfo<T>::fields;
    auto& tableName = ModelInfo<T>::tableName;
    std::ostringstream sql;
    
    sql << "UPDATE " << tableName << " SET ";
    
    // Add field assignments (skip id field typically)
    bool first = true;
    for (size_t i = 0; i < info.size(); ++i) {
        if (info[i].name == "id") continue; // Skip id field
        if (!first) sql << ", ";
        sql << info[i].name << " = ?";
        first = false;
    }
    
    sql << " WHERE id = ?;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    // Bind values
    const char* objPtr = reinterpret_cast<const char*>(&obj);
    int paramIndex = 1;
    
    for (size_t i = 0; i < info.size(); ++i) {
        if (info[i].name == "id") continue; // Skip id field in SET
        
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
    
    // Bind id for WHERE clause
    sqlite3_bind_int(stmt, paramIndex, id);
    
    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    
    return success;
}

}
