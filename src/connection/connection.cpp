//
// Created by gd1 on 07.09.2019.
//

#include <iostream>
#include "connection.h"
#include "boost/bind.hpp"
#include "../request/request.h"

HTTPConnection::HTTPConnection(boost::asio::io_service& io_service) : socket_(io_service) {};

std::shared_ptr<HTTPConnection> HTTPConnection::createConnection(boost::asio::io_service& io_service) {
    return std::make_shared<HTTPConnection>(io_service);
}

boost::asio::ip::tcp::socket& HTTPConnection::getSocket() {
    return socket_;
}

void HTTPConnection::startProcessing() {
    socket_.non_blocking(true);
    boost::asio::async_read_until(
            socket_,
            buf,
            "\r\n\r\n",
            boost::bind(
                    &HTTPConnection::someHandler,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred
                    ));
}

void HTTPConnection::someHandler(boost::system::error_code error, size_t bytes_transferred) {
    if (!error) {
        std::string data = boost::asio::buffer_cast<const char *>(buf.data());
        auto request = std::make_shared<HTTPRequest>();
        request->parseRequest(data);
    } else {
        std::cout << "got here";
        std::cout << error.message() << " " << error.value() << " (empty lines/without to newlines)" << std::endl;
    }
}

