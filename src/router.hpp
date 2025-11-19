#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include "http_request.hpp"
#include "http_response.hpp"

class Router {
    public:
        using Handler = std::function<HttpResponse(const HttpRequest&)>;

        static void get(const std::string& path, Handler handler);
        static void post(const std::string& path, Handler handler);
        static void put(const std::string& path, Handler handler);
        static void del(const std::string& path, Handler handler);

        static HttpResponse handle(const HttpRequest& req);

    private:
        static std::unordered_map<std::string, Handler> get_routes;
        static std::unordered_map<std::string, Handler> post_routes;
        static std::unordered_map<std::string, Handler> put_routes;
        static std::unordered_map<std::string, Handler> delete_routes;
        
        static bool matchPath(const std::string& pattern, const std::string& path);
        static Handler findHandler(const std::unordered_map<std::string, Handler>& routes, const std::string& path);
};