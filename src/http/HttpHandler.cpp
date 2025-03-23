#include "HttpHandler.h"
#include <unistd.h>
#include <string>
#include <stdexcept>

#include "HttpRequest.h"
#include "HttpResponse.h"


void HttpHandler::HandleClient(const int client_fd) {
    if (client_fd < 0) {
        throw std::runtime_error("Accept failed");
    }

    char buffer[1024]{};
    read(client_fd, buffer, sizeof(buffer));

    const HttpRequest request{buffer};
    HttpResponse response(request.GetVersion(), 404, R"({"error": "Not Found"})");

    if (request.GetMethod() == "GET") {
        response = HttpResponse(request.GetVersion(), 200, R"({"message": "GET request received"})");
    } else if (request.GetMethod() == "POST") {
        response = HttpResponse(request.GetVersion(), 200, R"({"message": "POST request received"})");
    }

    write(client_fd, response.ToString().c_str(), response.ToString().length());
    close(client_fd);
}
