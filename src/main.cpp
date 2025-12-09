#include <iostream>
#include <network/http_server.hpp>

int main() {
    HttpServer http_server(8080);
    http_server.Init();
    return 0;
}
