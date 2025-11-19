cmake_minimum_required(VERSION 3.10)
project(HttpServer)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/src)

# Find SQLite3
find_package(SQLite3 REQUIRED)

add_executable(http_server
    src/main.cpp
    src/server.cpp
    src/http_request.cpp
    src/http_response.cpp
    src/router.cpp
)

# Link SQLite3
target_link_libraries(http_server SQLite::SQLite3)

file(COPY ${CMAKE_SOURCE_DIR}/public DESTINATION ${CMAKE_BINARY_DIR})
