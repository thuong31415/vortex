#include "HttpResponse.h"

#include <utility>

HttpResponse::HttpResponse(std::string http_version, const int status_code, std::string body, std::string content_type)
    : http_version_(std::move(http_version)), status_code_(status_code), body_(std::move(body)) {
    headers_["Content-Type"] = std::move(content_type);
    headers_["Connection"] = "close";
    if (!body_.empty()) {
        headers_["Content-Length"] = std::to_string(body_.length());
    }
}

std::string HttpResponse::ToString() const {
    std::string response = http_version_ + " " + std::to_string(status_code_) + " OK\r\n";
    for (const auto &[key, value] : headers_) {
        response += key + ": " + value + "\r\n";
    }
    response += "\r\n";

    if (!body_.empty()) {
        response += body_;
    }

    return response;
}