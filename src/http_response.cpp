#include "http_response.hpp"
#include <sstream>
#include <unordered_map>

static std::string get_status_message(int status) {
    static const std::unordered_map<int, std::string> status_messages = {
        {200, "OK"},
        {201, "Created"},
        {204, "No Content"},
        {400, "Bad Request"},
        {404, "Not Found"},
        {500, "Internal Server Error"}
    };
    
    auto it = status_messages.find(status);
    if (it != status_messages.end()) {
        return it->second;
    }
    return "Unknown";
}

std::string HttpResponse::to_string() const {
    std::ostringstream out;

    out << "HTTP/1.1 " << status << " " << get_status_message(status) << "\r\n";
    out << "Content-Type: " << content_type << "\r\n";
    out << "Content-Length: " << body.size() << "\r\n";
    out << "\r\n";
    out << body;

    return out.str();
}