#pragma once
#include <string>

struct HttpResponse {
    int status;
    std::string body;
    std::string content_type = "text/plain";

    std::string to_string() const;
};