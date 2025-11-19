#include "http_request.hpp"
#include <sstream>

HttpMethod HttpRequest::to_method(const std::string& m){
    if (m == "GET") return HttpMethod::GET;
    if (m == "POST") return HttpMethod::POST;
    if (m == "PUT") return HttpMethod::PUT;
    if (m == "DELETE") return HttpMethod::DELETE;
    return HttpMethod::UNKNOWN;
}

HttpRequest HttpRequest::from_string(const std::string& raw) {
    // Creating the HttpRequest object to populate
    HttpRequest req;

    // Creating a stream from the raw request string in order to read it line by line
    std::istringstream stream(raw);
    // temporary string for line
    std::string line;

    // -------------
    // Parse request line
    // --------------
    // Reading first line of the request and storing it in 'line'
    std::getline(stream, line);
    // Creating a stream for the request line
    std::istringstream request_method_stream(line);

    std::string request_method_str;
    // Extracting method and a path from the request line. >> means we read until space and we take next part (word)
    request_method_stream >> request_method_str >> req.path;
    // Basic validation on request line
    if(request_method_str.empty() || req.path.empty()){
        throw std::runtime_error("Invalid HTTP request line");
    }
    // Converting method string to HttpMethod enum
    req.method = HttpRequest::to_method(request_method_str);

    // -------------
    // Parse headers
    // -------------
    while(std::getline(stream, line) && line != "\r") {
        std::size_t colon = line.find(':');
        if(colon != std::string::npos){
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 2); // skip ": "
            if(!value.empty() && value.back() == '\r')
                value.pop_back();
            req.headers[key] = value;
        }
    }

    // -------------
    // Parse body
    // -------------
    req.body.assign(std::istreambuf_iterator<char>(stream), {});

    return req;
}