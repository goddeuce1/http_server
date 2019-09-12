//
// Created by gd1 on 07.09.2019.
//

#include "server.h"
#include "../connection/connection.h"
#include "boost/bind.hpp"
#include <thread>

HTTPServer::HTTPServer(HTTPConfig *cfg) :
    acceptor_(
            io_service_,
            boost::asio::ip::tcp::endpoint(
                    boost::asio::ip::tcp::v4(),
                    cfg->getPort()
                    ))
{
    document_root = cfg->getDocumentRoot();
    cpu_limit = cfg->getCPULimit();
    thread_limit = cfg->getThreadLimit();

    server_name = "gdinx v1.0.0";
}

void HTTPServer::serverStart() {
    for (size_t i = 0; i < cpu_limit; ++i) {
        thread_group_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
        io_service_.post(boost::bind(&HTTPServer::serverListen, this));
    }

    thread_group_.join_all();
}

void HTTPServer::serverListen() {
    auto new_connection = std::make_shared<HTTPConnection>(io_service_, shared_from_this());
    acceptor_.async_accept(
            new_connection->getSocket(),
            [this, new_connection](boost::system::error_code error) {
                    if (!error) {
                        new_connection->startProcessing();
                    } else {
                        std::cout << error.message() << " " << error.value() << std::endl;
                    }
                    serverListen();
            }
    );
}

std::string HTTPServer::getDocRoot() {
    return document_root;
}

std::string HTTPServer::getName() {
    return server_name;
}