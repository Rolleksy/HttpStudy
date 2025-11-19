#pragma once
#include "router.hpp"

inline void register_delete_routes(){
    Router::del("/user", [](auto req){
        return HttpResponse{200, "User deleted"};
    });
}