//
// Created by gd1 on 11.09.2019.
//

#include "response.h"
#include "boost/filesystem.hpp"

HTTPResponse::HTTPResponse(std::shared_ptr<HTTPRequest> request) :
    request_(std::move(request))
    {
        response_headers["Server"] = request_->getConnection()->getServer()->getName();
        response_headers["Date"] = getDate();
        response_headers["Connection"] = "Closed";
    }

void HTTPResponse::startProcessing() {
    std::string method = request_->getMethod();

    if (method == "GET") {
        processGetMethod();
    } else if (method == "HEAD") {
        processHeadMethod();
    } else {
        processUnknownMethod();
    }

    writeHeaders(method);
}

void HTTPResponse::processGetMethod() {

}

void HTTPResponse::processHeadMethod() {

}

void HTTPResponse::processUnknownMethod() {
    response_code = "405 Method Not Allowed";
}

void HTTPResponse::writeToClient() {
    request_->getConnection()->getSocket().async_write_some(
            boost::asio::buffer(buffer.str(), 1024),
            [this](const boost::system::error_code& error, size_t bytes_transferred) {
                std::cout << bytes_transferred << std::endl;
            }
            );
}

void HTTPResponse::writeHeaders(std::string& method) {
    buffer << request_->getVersion() << " " << response_code << "\r\n";

    for (auto& header: response_headers) {
        buffer << header.first << ": " << header.second << "\r\n";
    }

    if (method == "GET") {
        buffer << std::string(response_file.begin(), response_file.end());
    }

    writeToClient();
}

std::string HTTPResponse::getDate() {
    std::time_t timer = std::time(nullptr);
    char buffer_time[256];
    auto time_now = std::strftime(buffer_time, sizeof(buffer_time), "%a, %d %b %Y %H:%M:%S GMT", std::localtime(&timer));

    return std::string(buffer_time, time_now);
}