#include "HttpRequest.h"

#include <sstream>

HttpRequest::HttpRequest(const std::string &raw_request) {
    ParseRequest(raw_request);
}

void HttpRequest::ParseRequest(const std::string &raw_request) {
    std::istringstream stream(raw_request);
    std::string line;

    // Parse request line
    if (std::getline(stream, line)) {
        std::istringstream line_stream(line);
        line_stream >> method_ >> path_ >> version_;
    }

    // Parse headers
    while (std::getline(stream, line) && !line.empty()) {
        // Remove possible '\r' from line endings (HTTP uses CRLF)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) break; // Ensure we stop at empty line

        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 2); // +2 to skip ": "
            headers_[key] = value;
        }
    }

    // Skip the empty line between headers and body
    if (stream.peek() == '\r') stream.get();
    if (stream.peek() == '\n') stream.get();

    // Parse body
    if (stream.rdbuf()->in_avail() > 0) {
        std::ostringstream body_stream;
        body_stream << stream.rdbuf();
        body_ = body_stream.str();
    }

}


std::string HttpRequest::GetMethod() const {
    return method_;
}

std::string HttpRequest::GetPath() const {
    return path_;
}

std::string HttpRequest::GetVersion() const {
    return version_;
}

std::string HttpRequest::GetHeader(const std::string &key) const {
    const auto it = headers_.find(key);
    return (it != headers_.end()) ? it->second : "";
}

std::string HttpRequest::GetBody() const {
    return body_;
}
