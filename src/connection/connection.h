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
    explicit HTTPConnection(boost::asio::io_service&, std::shared_ptr<HTTPServer>);
    ~HTTPConnection();
    void startProcessing();
    void stopProcessing();
    void readHandler(boost::system::error_code, size_t);

public:
    boost::asio::ip::tcp::socket& getSocket();
    std::shared_ptr<HTTPServer> getServer();

private:
    std::shared_ptr<HTTPServer> server_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buf;
};