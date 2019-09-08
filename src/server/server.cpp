//
// Created by gd1 on 07.09.2019.
//

#include "server.h"
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

    serverStart();
}

void HTTPServer::serverStart() {
    for (size_t i = 0; i < cpu_limit; ++i) {
        thread_group_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
        io_service_.post(boost::bind(&HTTPServer::serverListen, this));
    }

    thread_group_.join_all();
}

void HTTPServer::serverListen() {
    auto new_connection = std::make_shared<HTTPConnection>(io_service_);
    acceptor_.async_accept(
            new_connection->getSocket(),
            boost::bind(
                    &HTTPServer::handleConnection,
                    this,
                    new_connection,
                    boost::asio::placeholders::error
                    ));
}

void HTTPServer::handleConnection(std::shared_ptr<HTTPConnection> connection, boost::system::error_code error) {
    if (!error) {
        connection->startProcessing();
    } else {
        std::cout << error.message() << " " << error.value() << std::endl;
    }
    serverListen();
}



