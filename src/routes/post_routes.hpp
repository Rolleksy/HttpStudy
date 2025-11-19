#pragma once
#include "router.hpp"

inline void register_post_routes(){
    Router::post("/upload", [](auto req){
        return HttpResponse{200, "Received body: " + req.body};
    });
}