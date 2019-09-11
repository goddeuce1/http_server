//
// Created by gd1 on 07.09.2019.
//
#pragma once

#include <iostream>
#include <fstream>

#ifndef HTTP_SERVER_CONFIG_H
#define HTTP_SERVER_CONFIG_H

#endif //HTTP_SERVER_CONFIG_H

class HTTPConfig {
public:
    HTTPConfig();
    ~HTTPConfig();
    void parseConfig();
    std::string getPath();
    std::string getDocumentRoot();
    size_t getCPULimit();
    size_t getThreadLimit();
    size_t getPort();

private:
    std::string cfg_path;
    std::string document_root;
    size_t cpu_limit;
    size_t thread_limit;
    size_t port;
};