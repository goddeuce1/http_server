//
// Created by gd1 on 08.09.2019.
//

#include "request.h"
#include "../response/response.h"
#include <regex>
#include <chrono>
#include <thread>

HTTPRequest::HTTPRequest(std::shared_ptr<HTTPConnection> connection) :
connection_(std::move(connection))
{
    request_method = "";
    request_path = "";
    request_version = "";
}

void HTTPRequest::parseRequest(std::string request) {
    std::regex reg_body(R"(^(\w+)\s(\/((\S+\/)*)?)((\S+)|(\%\w{2})*)?((\?)((\S+\=\S+(\&)?)*))?\s(HTTP\/1\.[01])$)", std::regex_constants::icase | std::regex_constants::ECMAScript);
    std::regex reg_param(R"(^(\S+)\:\s(\S+)$)", std::regex_constants::icase | std::regex_constants::ECMAScript);

    size_t i = 0;
    request_path = connection_->getServer()->getDocRoot();

    do {
        size_t newline_position = request.find_first_of('\n', i);
        std::string request_body = request.substr(i, newline_position - i - 1);

        std::smatch match;

        if (std::regex_match(request_body, match, reg_body)) {
            request_method = match[1];
            request_path += match[2];
            request_file = match[6];
            request_version = match[13];
        }

        if (std::regex_match(request_body, match, reg_param)) {
            request_headers[match[1]] = match[2];
        }

        i = newline_position + 1;

    } while (request[i] != '\r');

    size_t args_pos = request_file.find_first_of('?');

    if (args_pos != std::string::npos) {
        request_file = request_file.substr(0, args_pos);
    }

    if (!request_file.empty()) {
        request_file = decodeUrl(request_file);
    } else {
        request_file = "index.html";
    }

    auto response = std::make_shared<HTTPResponse>(shared_from_this());
    response->startProcessing();
}

std::string HTTPRequest::decodeUrl(std::string input) {
    std::string result;

    for (std::size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '%') {

            if (i + 3 <= input.size()) {
                int value = 0;
                std::istringstream is(input.substr(i + 1, 2));

                if (is >> std::hex >> value) {
                    result += static_cast<char>(value);
                    i += 2;
                } else {
                    return result;
                }

            } else {
                return result;
            }

        } else if (input[i] == '+') {
            result += ' ';

        } else {
            result += input[i];
        }
    }
    return result;
}

std::shared_ptr<HTTPConnection> HTTPRequest::getConnection() {
    return connection_;
}

std::string HTTPRequest::getMethod() {
    return request_method;
}

std::string HTTPRequest::getPath() {
    return request_path;
}

std::string HTTPRequest::getFile() {
    return request_file;
}

std::string HTTPRequest::getVersion() {
    return request_version;
}