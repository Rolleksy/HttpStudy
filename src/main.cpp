#include "server.hpp"
#include "routes/routes.hpp"

int main() {
    register_routes();

    HttpServer server(8080);
    server.start();
    return 0;
}