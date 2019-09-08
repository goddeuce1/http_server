//
// Created by gd1 on 07.09.2019.
//



#ifndef HTTP_SERVER_SERVER_H
#define HTTP_SERVER_SERVER_H

#endif //HTTP_SERVER_SERVER_H

#include "boost/asio.hpp"
#include "boost/thread.hpp"
#include "../common/config.h"
#include "../connection/connection.h"

class HTTPServer : std::enable_shared_from_this<HTTPServer> {
public:
    explicit HTTPServer(HTTPConfig* cfg);
    //~HTTPServer();
    void serverStart();
    //void serverStop();

private:
    void serverListen();
    void handleConnection(std::shared_ptr<HTTPConnection> connection, boost::system::error_code error);

private:
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::thread_group thread_group_;
    std::string document_root;
    size_t cpu_limit;
    size_t thread_limit;
};