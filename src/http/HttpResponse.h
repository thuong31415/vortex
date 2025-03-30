#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <unordered_map>

class HttpResponse {
public:
    HttpResponse() = default;

    HttpResponse(std::string http_version, int status_code, std::string body,
                 std::string content_type = "application/json");

    [[nodiscard]] std::string ToString() const;

private:
    std::string http_version_{};
    int status_code_{};
    std::string body_{};
    std::unordered_map<std::string, std::string> headers_{};
};

#endif //HTTP_RESPONSE_H
