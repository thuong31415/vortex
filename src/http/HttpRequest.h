#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <unordered_map>


class HttpRequest {
public:

    HttpRequest() = default;

    explicit HttpRequest(const std::string &raw_request);

    std::string GetMethod() const;

    std::string GetPath() const;

    std::string GetHeader(const std::string &key) const;

    std::string GetBody() const;

    std::string GetVersion() const;

private:
    std::string method_{};
    std::string path_{};
    std::string version_{};
    std::string body_{};
    std::unordered_map<std::string, std::string> headers_{};

    void ParseRequest(const std::string &raw_request);
};

#endif //HTTP_REQUEST_H
