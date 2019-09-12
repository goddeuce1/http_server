//
// Created by gd1 on 11.09.2019.
//
#pragma once

#include "../request/request.h"

#ifndef HTTP_SERVER_RESPONSE_H
#define HTTP_SERVER_RESPONSE_H

#endif //HTTP_SERVER_RESPONSE_H

class HTTPResponse {
public:
    explicit HTTPResponse(std::shared_ptr<HTTPRequest>);
    //~HTTPResponse();
    void startProcessing();
    void processGetMethod();
    void processHeadMethod();
    void processUnknownMethod();

public:
    static std::string getDate();

private:
    void writeHeaders(std::string&);
    void writeToClient();

private:
    std::shared_ptr<HTTPRequest> request_;
    std::stringstream buffer;
    std::string response_code;
    std::vector<char> response_file;
    std::unordered_map<std::string, std::string> response_headers;
};