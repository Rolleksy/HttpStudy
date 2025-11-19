#pragma once
#include <string>
#include "http_request.hpp"
#include "http_response.hpp"

class HttpServer {
    public:
        HttpServer(int port);
        void start();

    private:
        int port_;
        int server_fd_;
        void handle_client(int client_id);
        HttpRequest parse_request(const std::string& raw);
};

