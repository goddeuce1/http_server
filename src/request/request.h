//
// Created by gd1 on 08.09.2019.
//
#pragma once

#include "../connection/connection.h"
#include "boost/asio.hpp"
#include <iostream>
#include <unordered_map>

#ifndef HTTP_SERVER_REQUEST_H
#define HTTP_SERVER_REQUEST_H

#endif //HTTP_SERVER_REQUEST_H

class HTTPRequest : public std::enable_shared_from_this<HTTPRequest> {
public:
    explicit HTTPRequest(std::shared_ptr<HTTPConnection>);
    ~HTTPRequest() = default;
    void parseRequest(std::string);
    static std::string decodeUrl(std::string);

public:
    std::shared_ptr<HTTPConnection> getConnection();
    std::string getMethod();
    std::string getPath();
    std::string getFile();
    std::string getVersion();

private:
    std::shared_ptr<HTTPConnection> connection_;
    std::unordered_map<std::string, std::string> request_headers;
    std::string request_method;
    std::string request_path;
    std::string request_file;
    std::string request_version;
};