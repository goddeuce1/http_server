//
// Created by gd1 on 07.09.2019.
//
#pragma once

#ifndef HTTP_SERVER_SERVER_H
#define HTTP_SERVER_SERVER_H

#endif //HTTP_SERVER_SERVER_H

#include "../common/config.h"
#include "boost/asio.hpp"
#include "boost/thread.hpp"

class HTTPServer : public std::enable_shared_from_this<HTTPServer> {
public:
    explicit HTTPServer(HTTPConfig* cfg);
    //~HTTPServer();
    void serverStart();
    std::string getDocRoot();
    //void serverStop();

private:
    void serverListen();

private:
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::thread_group thread_group_;
    std::string document_root;
    size_t cpu_limit;
    size_t thread_limit;
};