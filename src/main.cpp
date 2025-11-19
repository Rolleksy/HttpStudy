#include "server.hpp"
#include "routes/routes.hpp"
#include "db.hpp"

int main() {
    // Initialize database
    DB::getInstance().init("app.db");
    
    register_routes();

    HttpServer server(8080);
    server.start();
    return 0;
}