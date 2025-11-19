#pragma once
#include <sqlite3.h>
#include <string>
#include <stdexcept>

class DB {
public:
    static DB& getInstance() {
        static DB instance;
        return instance;
    }

    sqlite3* get() { return db_; }

    void init(const std::string& dbPath = "app.db") {
        if (sqlite3_open(dbPath.c_str(), &db_) != SQLITE_OK) {
            throw std::runtime_error("Failed to open database");
        }
    }

    ~DB() {
        if (db_) {
            sqlite3_close(db_);
        }
    }

private:
    DB() : db_(nullptr) {}
    DB(const DB&) = delete;
    DB& operator=(const DB&) = delete;

    sqlite3* db_;
};
