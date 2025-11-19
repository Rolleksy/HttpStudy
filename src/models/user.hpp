
#pragma once // Modern include guard, prevents double inclusion
#include "orm/model.hpp" // Includes ORM macros for model mapping
#include <string>

// User model struct
struct User {
    int id; // Primary key, auto-incremented
    std::string name; // User's name
    int age; // User's age
};

// ORM macros below use C++ metaprogramming to register fields and offsets for generic CRUD
ORM_BEGIN(User)
    ORM_FIELD(User, id)   // Registers 'id' field for ORM
    ORM_FIELD(User, name) // Registers 'name' field for ORM
    ORM_FIELD(User, age)  // Registers 'age' field for ORM
ORM_END()  // Ends ORM registration for User