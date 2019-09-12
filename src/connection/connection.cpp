//
// Created by gd1 on 07.09.2019.
//

#include "connection.h"
#include "../request/request.h"
#include <iostream>
#include "boost/bind.hpp"


HTTPConnection::HTTPConnection(boost::asio::io_service& io_service, std::shared_ptr<HTTPServer> server) :
    socket_(io_service),
    server_(std::move(server))
    {};

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
    if (error || bytes_transferred == 0) {
        std::cout << error.message() << " " << error.value() << " (empty lines/without to newlines)" << std::endl;
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