#pragma once
#include "routes/get_routes.hpp"
#include "routes/post_routes.hpp"
#include "routes/put_routes.hpp"
#include "routes/delete_routes.hpp"

inline void register_routes() {
    register_get_routes();
    register_post_routes();
    register_put_routes();
    register_delete_routes();
}
