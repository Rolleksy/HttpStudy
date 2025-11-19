#pragma once
#include <fstream>
#include <sstream>

inline std::string load_files(const std::string html_name, const std:: string css_name){
    std::string files_path = "public/";
    std::string full_html_path = files_path + html_name + ".html";
    std::string full_css_path = files_path + css_name + ".css";
    
    std::ifstream html_file(full_html_path);
    std::ifstream css_file(full_css_path);

    if (!html_file.is_open() || !css_file.is_open()) {
        throw std::runtime_error("Failed to open HTML or CSS file");
    }

    std::stringstream html_buffer;
    std::stringstream css_buffer;

    if(!html_file.good() || !css_file.good()) {
        throw std::runtime_error("Error reading HTML or CSS file");
    }

    html_buffer << html_file.rdbuf();
    css_buffer << css_file.rdbuf();

    std::string full_html = R"(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>My HTTP Server</title>
        <style>
    )" + css_buffer.str() + R"(
        </style>
    </head>
    <body>
    )" + html_buffer.str() + R"(
    </body>
    </html>
    )";

    return full_html;
}