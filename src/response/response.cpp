//
// Created by gd1 on 11.09.2019.
//

#include "response.h"
#include "boost/filesystem.hpp"

HTTPResponse::HTTPResponse(std::shared_ptr<HTTPRequest> request) :
    request_(std::move(request)),
    ostream_to_buffer(&response_buffer)
    {
        response_headers["Server"] = request_->getConnection()->getServer()->getName();
        response_headers["Date"] = getDate();
        response_headers["Connection"] = "Closed";

        is_sent = false;
    }

void HTTPResponse::startProcessing() {
    std::string method = request_->getMethod();

    if (method == "GET" || method == "HEAD") {
        processMethod();
    } else {
        processUnknownMethod();
    }

    writeHeaders(method,response_code);
}

void HTTPResponse::processMethod() {
    std::string file_path = request_->getPath();
    std::string file_name = request_->getFile();

    size_t escape_index = file_path.find("../");

    if (boost::filesystem::exists(file_path + file_name) && escape_index == std::string::npos) {
        size_t dot_position = file_name.find_last_of('.');
        std::string file_extension = file_name.substr(dot_position + 1, file_name.length() - dot_position);
        std::string content_type = getContentType(file_extension);

        response_headers["Content-Type"] = content_type;
        response_headers["Content-Length"] = std::to_string(boost::filesystem::file_size(file_path + file_name));
        response_code = "200 OK";

        return;
    } else if (escape_index != std::string::npos || file_name == "index.html") {
        response_code = "403 Forbidden";
        return;
    }

    response_code = "404 Not Found";
}

void HTTPResponse::processUnknownMethod() {
    response_code = "405 Method Not Allowed";
}

void HTTPResponse::writeHeaders(std::string& method, std::string& code) {
    ostream_to_buffer << request_->getVersion() << " " << response_code << "\r\n";

    for (auto& header: response_headers) {
        ostream_to_buffer << header.first << ": " << header.second << "\r\n";
    }

    if (method == "HEAD") {
        ostream_to_buffer << "\r\n";
    }

    if (method == "GET" && code == "200 OK") {
        ostream_to_buffer << "\r\n";
        sendFile();
    }

    writeToClient();
}

void HTTPResponse::sendFile() {
    std::ifstream file(request_->getPath() + request_->getFile(), std::ios_base::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});
    std::copy(buffer.cbegin(), buffer.cend(), std::ostreambuf_iterator<char>(ostream_to_buffer));
}

void HTTPResponse::writeToClient() {
    boost::asio::async_write(
            request_->getConnection()->getSocket(),
            response_buffer,
            [this](const boost::system::error_code& error, size_t bytes_transferred) {
                if (error) {
                    std::cout << error.message() << " || " << error.value() << " || " << response_code << std::endl;
                    return;
                }

                std::cout << "bytes transferred: " << bytes_transferred << std::endl;
                is_sent = true;
            }
    );
}

std::string HTTPResponse::getDate() {
    std::time_t timer = std::time(nullptr);
    char buffer_time[256];
    auto time_now = std::strftime(buffer_time, sizeof(buffer_time), "%a, %d %b %Y %H:%M:%S GMT", std::localtime(&timer));

    return std::string(buffer_time, time_now);
}

std::string HTTPResponse::getContentType(std::string& extension) {
    if (extension == "html") return "text/html";
    if (extension == "css") return "text/css";
    if (extension == "js") return "application/javascript";
    if (extension == "jpg") return "image/jpeg";
    if (extension == "jpeg") return "image/jpeg";
    if (extension == "png") return "image/png";
    if (extension == "gif") return "image/gif";
    if (extension == "swf") return "application/x-shockwave-flash";
    if (extension == "txt") return "text/plain";

    return "";
}

bool HTTPResponse::isFileSend() {
    return is_sent;
}