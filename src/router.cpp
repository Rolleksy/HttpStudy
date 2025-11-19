#include "router.hpp"
#include <sstream>
#include <vector>
#include <iostream>

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

// Simple path matching: supports {param} syntax
bool Router::matchPath(const std::string& pattern, const std::string& path) {
    // Split both into segments
    auto splitPath = [](const std::string& p) {
        std::vector<std::string> segments;
        std::stringstream ss(p);
        std::string segment;
        while (std::getline(ss, segment, '/')) {
            if (!segment.empty()) {
                segments.push_back(segment);
            }
        }
        return segments;
    };
    
    auto patternSegs = splitPath(pattern);
    auto pathSegs = splitPath(path);
    
    if (patternSegs.size() != pathSegs.size()) {
        return false;
    }
    
    for (size_t i = 0; i < patternSegs.size(); ++i) {
        const auto& pat = patternSegs[i];
        const auto& seg = pathSegs[i];
        
        // Check if it's a parameter (starts with { and ends with })
        if (pat.size() > 2 && pat.front() == '{' && pat.back() == '}') {
            continue; // Parameter matches anything
        }
        
        // Exact match required
        if (pat != seg) {
            return false;
        }
    }
    
    return true;
}

Router::Handler Router::findHandler(const std::unordered_map<std::string, Handler>& routes, const std::string& path) {
    // First try exact match
    auto it = routes.find(path);
    if (it != routes.end()) {
        return it->second;
    }
    
    // Then try pattern matching
    std::cerr << "[DEBUG] Searching for path: " << path << std::endl;
    std::cerr << "[DEBUG] Available routes:" << std::endl;
    for (const auto& [pattern, handler] : routes) {
        std::cerr << "  - " << pattern;
        if (matchPath(pattern, path)) {
            std::cerr << " [MATCH]" << std::endl;
            return handler;
        }
        std::cerr << " [NO MATCH]" << std::endl;
    }
    
    return nullptr;
}

HttpResponse Router::handle(const HttpRequest& req) {
    auto method = req.method;
    try
    {
        Handler handler = nullptr;
        
        switch (req.method){
            case HttpMethod::GET:
                handler = findHandler(get_routes, req.path);
                break;
            case HttpMethod::POST:
                handler = findHandler(post_routes, req.path);
                break;
            case HttpMethod::PUT:
                handler = findHandler(put_routes, req.path);
                break;
            case HttpMethod::DELETE:
                handler = findHandler(delete_routes, req.path);
                std::cerr << "[DEBUG] DELETE request for path: " << req.path << std::endl;
                std::cerr << "[DEBUG] Handler found: " << (handler ? "yes" : "no") << std::endl;
                break;
            default:
                return HttpResponse{405, "Method Not Allowed"};
        }
        
        if (handler) {
            return handler(req);
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
    
