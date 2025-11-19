#include "router.hpp"

std::unordered_map<std::string, Router::Handler> Router::get_routes;
std::unordered_map<std::string, Router::Handler> Router::post_routes;
std::unordered_map<std::string, Router::Handler> Router::put_routes;
std::unordered_map<std::string, Router::Handler> Router::delete_routes;

void Router::get(const std::string& path, Handler handler) {
    get_routes[path] = handler;
}

void Router::post(const std::string& path, Handler handler) {
    post_routes[path] = handler;
}

void Router::put(const std::string& path, Handler handler) {
    put_routes[path] = handler;
}

void Router::del(const std::string& path, Handler handler) {
    delete_routes[path] = handler;
}

HttpResponse Router::handle(const HttpRequest& req) {
    auto method = req.method;
    try
    {
        switch (req.method){
            case HttpMethod::GET: {
                if (get_routes.count(req.path))
                    return get_routes[req.path](req);
                break;
            }
            case HttpMethod::POST: {
                if (post_routes.count(req.path))
                    return post_routes[req.path](req);
                break;
            }
            case HttpMethod::PUT: {
                if (put_routes.count(req.path))
                    return put_routes[req.path](req);
                break;
            }
            case HttpMethod::DELETE: {
                if (delete_routes.count(req.path))
                    return delete_routes[req.path](req);
                break;
            }
            default: {
                return HttpResponse{405, "Method Not Allowed"};
                break;
            }
        }
        return HttpResponse{404, "Not Found"};
       
    }
    catch(const std::exception& e)
    {
        return HttpResponse{500, std::string("Internal Server Error: ") + e.what()};
    }
    catch(...)
    {
        return HttpResponse{500, "Internal Server Error: Unknown error"};
    }
}
    
