//
// Created by gd1 on 11.09.2019.
//
#pragma once

#include "../request/request.h"

#ifndef HTTP_SERVER_RESPONSE_H
#define HTTP_SERVER_RESPONSE_H

#endif //HTTP_SERVER_RESPONSE_H

class HTTPResponse : public std::enable_shared_from_this<HTTPResponse>  {
public:
    explicit HTTPResponse(std::shared_ptr<HTTPRequest>);
    ~HTTPResponse() = default;
    void startProcessing();
    void processMethod();
    void processUnknownMethod();

public:
    static std::string getDate();
    static std::string getContentType(std::string&);

private:
    void writeHeaders(std::string&, std::string&);
    void writeToClient();
    void sendFile();

private:
    std::shared_ptr<HTTPRequest> request_;
    std::string response_code;
    std::unordered_map<std::string, std::string> response_headers;
    std::ostream ostream_to_buffer;
    boost::asio::streambuf response_buffer;
};