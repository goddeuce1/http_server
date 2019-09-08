#include "server/server.h"

int main() {
    HTTPConfig cfg_file;
    cfg_file.parseConfig();
    auto server = std::make_shared<HTTPServer>(&cfg_file);

    return 0;
}