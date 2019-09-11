//
// Created by gd1 on 07.09.2019.
//

#pragma once

#include "../server/server.h"
#include "boost/asio.hpp"
#include <boost/array.hpp>

#ifndef HTTP_SERVER_CONNECTION_H
#define HTTP_SERVER_CONNECTION_H

#endif //HTTP_SERVER_CONNECTION_H

class HTTPConnection : public std::enable_shared_from_this<HTTPConnection> {
public:
    explicit HTTPConnection(boost::asio::io_service& io_service, std::shared_ptr<HTTPServer> server);
    //~HTTPConnection();
    boost::asio::ip::tcp::socket& getSocket();
    std::shared_ptr<HTTPServer> getServer();
    void startProcessing();
    void someHandler(boost::system::error_code error, size_t bytes_transferred);

private:
    boost::asio::ip::tcp::socket socket_;
    std::shared_ptr<HTTPServer> server_;
    boost::asio::streambuf buf;
};