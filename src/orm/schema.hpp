
#pragma once
#include <sqlite3.h>
#include <sstream>
#include "orm/model.hpp" // Includes ORM macros and metadata


namespace orm {

// Template metaprogramming: uses ModelInfo<T> to get field metadata for any model type
template<typename T>
void createTable(sqlite3* db) {
    auto& info = ModelInfo<T>::fields; // Vector of field metadata (name, type, offset)
    std::ostringstream sql;

    // Dynamically build SQL CREATE TABLE statement using model metadata
    sql << "CREATE TABLE IF NOT EXISTS " << ModelInfo<T>::tableName << " (";

    for (size_t i = 0; i < info.size(); ++i) {
        sql << info[i].name << " " << info[i].sqliteType;
        // C++ trick: check for 'id' field and make it auto-increment primary key
        if (info[i].name == "id") {
            sql << " PRIMARY KEY AUTOINCREMENT";
        }
        if (i != info.size() - 1) sql << ", ";
    }
    sql << ");";

    char* errMsg = nullptr;
    // Error handling: throw exception if table creation fails
    if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = errMsg ? errMsg : "Unknown error";
        sqlite3_free(errMsg);
        throw std::runtime_error("Failed to create table: " + error);
    }
}

} // namespace orm
