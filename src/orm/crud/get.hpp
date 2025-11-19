
#pragma once
#include <sqlite3.h>
#include <optional>
#include <sstream>
#include "orm/model.hpp" // Includes ORM macros and model metadata

namespace orm {

// Uses C++ template metaprogramming to generically fetch a row by id
template<typename T>
std::optional<T> getById(sqlite3* db, int id) {
    auto& info = ModelInfo<T>::fields; // Field metadata (name, type, offset)
    auto& tableName = ModelInfo<T>::tableName;
    std::ostringstream sql;
    
    // Build SELECT query dynamically
    sql << "SELECT ";
    for (size_t i = 0; i < info.size(); ++i) {
        sql << info[i].name;
        if (i != info.size() - 1) sql << ", ";
    }
    sql << " FROM " << tableName << " WHERE id = ?;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }
    T obj;
    char* objPtr = reinterpret_cast<char*>(&obj); // Magic: treat struct as byte array
    for (size_t i = 0; i < info.size(); ++i) {
        const auto& field = info[i];
        char* fieldPtr = objPtr + field.offset; // Pointer arithmetic to field
        // Type-punning: write value to struct field using offset and type info
        if (std::string(field.sqliteType) == "INTEGER") {
            int value = sqlite3_column_int(stmt, i);
            *reinterpret_cast<int*>(fieldPtr) = value;
        } else if (std::string(field.sqliteType) == "TEXT") {
            const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            if (text) {
                *reinterpret_cast<std::string*>(fieldPtr) = std::string(text);
            }
        }
    }
    sqlite3_finalize(stmt);
    return obj;
}

// Uses template metaprogramming and pointer arithmetic to fetch all rows generically
template<typename T>
std::vector<T> getAll(sqlite3* db) {
    auto& info = ModelInfo<T>::fields;
    auto& tableName = ModelInfo<T>::tableName;
    std::ostringstream sql;
    sql << "SELECT ";
    for (size_t i = 0; i < info.size(); ++i) {
        sql << info[i].name;
        if (i != info.size() - 1) sql << ", ";
    }
    sql << " FROM " << tableName << ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return {};
    }
    std::vector<T> results;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        T obj;
        char* objPtr = reinterpret_cast<char*>(&obj); // Treat struct as byte array
        for (size_t i = 0; i < info.size(); ++i) {
            const auto& field = info[i];
            char* fieldPtr = objPtr + field.offset; // Pointer arithmetic to field
            // Type-punning: write value to struct field using offset and type info
            if (std::string(field.sqliteType) == "INTEGER") {
                int value = sqlite3_column_int(stmt, i);
                *reinterpret_cast<int*>(fieldPtr) = value;
            } else if (std::string(field.sqliteType) == "TEXT") {
                const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
                if (text) {
                    *reinterpret_cast<std::string*>(fieldPtr) = std::string(text);
                }
            }
        }
        results.push_back(obj);
    }
    sqlite3_finalize(stmt);
    return results;
}

} // namespace orm