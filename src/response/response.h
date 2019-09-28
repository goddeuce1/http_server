//
// Created by gd1 on 11.09.2019.
//

#include "../common/header.h"
#include "boost/asio.hpp"
#include <vector>
#include <iostream>
#include <unordered_map>

#ifndef HTTP_SERVER_RESPONSE_H
#define HTTP_SERVER_RESPONSE_H

#endif //HTTP_SERVER_RESPONSE_H

class HTTPResponse : public std::enable_shared_from_this<HTTPResponse>  {
public:
    explicit HTTPResponse() = default;
    ~HTTPResponse() = default;
    static std::string startProcessing(std::string&, std::string&, std::string&, char&);
    static std::string processMethod(std::string&, std::string&, std::string&, char&, std::vector<header>& headers);
    static std::string processUnknownMethod();
    static void initHeaders(std::vector<header>&);
    static void writeHeaders(std::string&, std::string&, std::string&, std::string&, std::vector<header>&);

public:
    static std::string getDate();
    static std::string getContentType(std::string&);
};