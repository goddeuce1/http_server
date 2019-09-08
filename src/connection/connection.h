//
// Created by gd1 on 07.09.2019.
//

#include "boost/asio.hpp"
#include <boost/array.hpp>

#ifndef HTTP_SERVER_CONNECTION_H
#define HTTP_SERVER_CONNECTION_H

#endif //HTTP_SERVER_CONNECTION_H

class HTTPConnection : public std::enable_shared_from_this<HTTPConnection> {
public:
    explicit HTTPConnection(boost::asio::io_service& io_service);
    //~HTTPConnection();
    static std::shared_ptr<HTTPConnection> createConnection(boost::asio::io_service& io_service);
    boost::asio::ip::tcp::socket& getSocket();
    void startProcessing();
    void someHandler(boost::system::error_code error, size_t bytes_transferred);

private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buf;
};