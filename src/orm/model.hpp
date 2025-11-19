#pragma once
#include <string>
#include <vector>
#include <tuple>

struct FieldInfo {
    std::string name;
    std::string sqliteType;
    size_t offset;
};

template<typename T>
struct ModelInfo {
    inline static std::string tableName;
    inline static std::vector<FieldInfo> fields;
};

#define ORM_BEGIN(TYPE) \
    template<> struct ModelInfo<TYPE> { \
        inline static std::string tableName = #TYPE; \
        inline static std::vector<FieldInfo> fields = {

#define ORM_FIELD(TYPE, FIELD) \
            { #FIELD, orm::sqliteType<decltype(TYPE::FIELD)>(), offsetof(TYPE, FIELD)},

#define ORM_END() \
        }; \
    };

namespace orm {
    template<typename T>
    constexpr const char* sqliteType();
    template<>
    constexpr const char* sqliteType<int>() { return "INTEGER"; }
    template<>
    constexpr const char* sqliteType<std::string>() { return "TEXT"; }
}
