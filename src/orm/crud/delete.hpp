#pragma once
#include "orm/model.hpp"
#include <sqlite3.h>
#include <sstream>

namespace orm {

template<typename T>
bool remove(sqlite3* db, int id) {
    auto& tableName = ModelInfo<T>::tableName;
    std::ostringstream sql;
    
    sql << "DELETE FROM " << tableName << " WHERE id = ?;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    
    return success;
}

}
