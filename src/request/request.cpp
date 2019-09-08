//
// Created by gd1 on 08.09.2019.
//

//match[0] - request
//match[1] - method
//match[2] - full path
//match[5] - item name
//match[8] - args with ?
//match[9] - ?
//match[10] - args without ?
//match[13] - http with version

#include "request.h"
#include <regex>

HTTPRequest::HTTPRequest() {
    params["Method"] = "";
    params["Path"] = "";
    params["File"] = "";
    params["HTTP"] = "";
}

void HTTPRequest::parseRequest(std::string request) {
    std::regex reg_body(R"(^(\w+)\s(\/((\S+\/)*)?)((\S+\.\w+)|(\%\w{2})*)?((\?)((\S+\=\S+(\&)?)*))?\s(HTTP\/1\.[01])$)", std::regex_constants::icase | std::regex_constants::ECMAScript);
    std::regex reg_param(R"(^(\S+)\:\s(\S+)$)", std::regex_constants::icase | std::regex_constants::ECMAScript);

    size_t i = 0;

    do {
        size_t newline_position = request.find_first_of('\n', i);
        std::string request_body;

        request_body = request.substr(i, newline_position - i - 1);

        std::smatch match;

        if (std::regex_match(request_body, match, reg_body)) {
            params["Method"] = match[1];
            params["Path"] = match[2];
            params["File"] = match[5];
            params["HTTP"] = match[13];
        }

        if (std::regex_match(request_body, match, reg_param)) {
            params[match[1]] = match[2];
        }

        i = newline_position + 1;

    } while (request[i] != '\r');

    if (!params["File"].empty()) {
        params["File"] = decodeUrl(params["File"]);
    }
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

