//
// Created by gd1 on 07.09.2019.
//

#include "connection.h"
#include "../request/request.h"
#include "../response/response.h"
#include <iostream>
#include "boost/bind.hpp"


HTTPConnection::HTTPConnection(boost::asio::ip::tcp::socket socket, std::shared_ptr<HTTPServer> server) :
    socket_(std::move(socket)),
    server_(std::move(server))
    {};

HTTPConnection::~HTTPConnection() {
    stopProcessing();
}

void HTTPConnection::stopProcessing() {
    socket_.cancel();
    socket_.close();
}

void HTTPConnection::startProcessing() {
    socket_.non_blocking(true);
    boost::asio::async_read_until(
            socket_,
            buf,
            "\r\n\r\n",
            boost::bind(
                    &HTTPConnection::readHandler,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred
                    ));
}

void HTTPConnection::readHandler(boost::system::error_code error, size_t bytes_transferred) {
    if (error) {

        if (bytes_transferred == 0) {
            return;
        }

        std::cout << error.message() << " " << error.value() << std::endl;
        return;
    } else {
        std::string data = boost::asio::buffer_cast<const char *>(buf.data());
        auto request = std::make_shared<HTTPRequest>(shared_from_this());
        request->parseRequest(data);
    }
}

boost::asio::ip::tcp::socket& HTTPConnection::getSocket() {
    return socket_;
}

std::shared_ptr<HTTPServer> HTTPConnection::getServer() {
    return server_;
}