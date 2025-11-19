#include "server.hpp"
#include "router.hpp"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

HttpServer::HttpServer(int port) : port_(port) {}

void HttpServer::start() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0){
        throw std::runtime_error("Failed to create socket");
    }

    // Allow socket reuse to prevent "Address already in use" errors
    int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("Failed to set socket options");
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port_);
    address.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd_, (sockaddr*)&address, sizeof(address)) < 0){
        throw std::runtime_error("Bind failed");
    }
    if(listen(server_fd_, 10) < 0){
        throw std::runtime_error("Listen failed");
    }

    std::cout << "Server running on port " << port_ << "...\n";

    while(true){
        int client_fd = accept(server_fd_, nullptr, nullptr);
        if(client_fd < 0){
            std::cerr << "Failed to accept connection\n";
            continue;
        }
        try
        {
            handle_client(client_fd);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error handling client: " << e.what() << '\n';
        }
        
        close(client_fd);
    }
}

void HttpServer::handle_client(int client_fd) {
    constexpr size_t buffer_size = 8192;
    char buffer[buffer_size];
    int bytes = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes <= 0) return;

    buffer[bytes] = '\0';
    std::string raw_request(buffer);

    HttpResponse response;

    try
    {
        HttpRequest request = parse_request(raw_request);
        response = Router::handle(request);
    }
    catch(const std::exception& e)
    {
        response = HttpResponse{400, "Bad Request: " + std::string(e.what())};
    }
    
    std::string raw_response = response.to_string();
    ssize_t written = write(client_fd, raw_response.c_str(), raw_response.size());
    
    if(written < 0){
        std::cerr << "Failed to send response to client\n";
    }
}

HttpRequest HttpServer::parse_request(const std::string& raw) {
    return HttpRequest::from_string(raw);
}