#pragma once
#include <string>
#include <map>

enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE,
    UNKNOWN
};

struct HttpRequest {
    HttpMethod method;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string body;

    static HttpRequest from_string(const std::string& raw);
    static HttpMethod to_method(const std::string& m);
};