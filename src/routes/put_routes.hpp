#pragma once
#include "router.hpp"

inline void register_put_routes(){
    Router::put("/user", [](auto res) {
        return HttpResponse{200, "User updated with data: " + res.body};
    });
}