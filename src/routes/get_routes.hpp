#pragma once 
#include "router.hpp"
#include "routes/website.hpp"


inline void register_get_routes(){
    Router::get("/", [](auto req){
        return HttpResponse{200, "hello from http server, on GET"};
    });

    Router::get("/website", [](auto req){
        std::string html = load_files(
            "index",
            "style"
        );
        return HttpResponse{200, html, "text/html"};
    });

    Router::get("/home", [](auto req){
        std::string html = load_files(
            "home",
            "home_style"
        );
        return HttpResponse{200, html, "text/html"};
    });
}