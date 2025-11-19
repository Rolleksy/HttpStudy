# HttpServer

A simple HTTP server implementation in C++ for learning purposes.

## Features

- Custom HTTP request and response handling
- Route-based request routing with support for GET, POST, PUT, and DELETE methods
- Modular architecture with separate route definitions
- Built with C++17 standard

## Project Structure

```
HttpStudy/
├── public/
|   ├── index.html
|   ├── style.css
├── src/
│   ├── main.cpp              # Entry point
│   ├── server.cpp/hpp        # HTTP server implementation
│   ├── router.cpp/hpp        # Request routing logic
│   ├── http_request.cpp/hpp  # HTTP request parsing
│   ├── http_response.cpp/hpp # HTTP response formatting
│   └── routes/               # Route definitions
│       ├── routes.hpp        # Route registration
│       ├── get_routes.hpp    # GET endpoints
│       ├── post_routes.hpp   # POST endpoints
│       ├── put_routes.hpp    # PUT endpoints
│       ├── delete_routes.hpp # DELETE endpoints
|       └── website.hpp       # Service parsing html and css files for GET/website
├── CMakeLists.txt
└── README.md
```

## Building

The project uses CMake as its build system. To build the project:

```bash
mkdir build
cd build
cmake ..
make
```

## Running

After building, run the server from the `build` directory:

```bash
./http_server
```

The server will start listening on port `8080`.

## Example Routes

- `GET /` - Returns a simple hello message
- `POST /upload` - Echoes back the request body
- `GET /website` - Returns `index.html` with its `style.css`

## Adding New Routes

To add new routes, edit the corresponding file in `src/routes/`:

- `get_routes.hpp` for GET endpoints
- `post_routes.hpp` for POST endpoints
- `put_routes.hpp` for PUT endpoints
- `delete_routes.hpp` for DELETE endpoints

Example:

```cpp
Router::get("/hello", [](auto req){
    return HttpResponse{200, "Hello, World!"};
});
```

## Requirements

- C++17 compatible compiler
- CMake 3.10 or higher
- Linux/Unix environment (uses POSIX sockets)

## Learning Goals

This project demonstrates:
- Basic HTTP protocol implementation
- Socket programming in C++
- Request routing patterns
- Modular C++ project structure
- Lambda functions and std::function
