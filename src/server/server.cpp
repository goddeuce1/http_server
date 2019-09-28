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
                    )),
                    connection_()
{
    document_root = cfg->getDocumentRoot();
    cpu_limit = cfg->getCPULimit();
    thread_limit = cfg->getThreadLimit();

    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.listen();

    serverListen();
}

HTTPServer::~HTTPServer() {
    serverStop();
}

void HTTPServer::serverStop() {
    acceptor_.cancel();
    acceptor_.close();
}

void HTTPServer::serverStart() {
    for (size_t i = 0; i < thread_limit; ++i) {
        thread_group_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
    }

    thread_group_.join_all();
}

void HTTPServer::serverListen() {
    connection_.reset(new HTTPConnection(io_service_, document_root));
    acceptor_.async_accept(
            connection_->getSocket(),
            boost::bind(&HTTPServer::listenHandler,
                    this,
                    boost::asio::placeholders::error
                    )
    );
}

void HTTPServer::listenHandler(boost::system::error_code error) {
    if (!error) {
        connection_->startProcessing();
    } else {
        std::cout << "Failed to accept new connection: " << error.message() << " " << error.value() << std::endl;
    }

    serverListen(); // not a recursion!
}