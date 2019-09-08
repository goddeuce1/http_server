//
// Created by gd1 on 08.09.2019.
//

#include "boost/asio.hpp"
#include <iostream>
#include <unordered_map>

#ifndef HTTP_SERVER_REQUEST_H
#define HTTP_SERVER_REQUEST_H

#endif //HTTP_SERVER_REQUEST_H

class HTTPRequest : public std::enable_shared_from_this<HTTPRequest> {
public:
    HTTPRequest();
    //~HTTPRequest();
    void parseRequest(std::string request);
    std::string decodeUrl(std::string input);

private:
    std::unordered_map<std::string, std::string> params;
};