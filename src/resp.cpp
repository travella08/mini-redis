#include "resp.h"

namespace resp {

std::string simple_string(const std::string& s) {
    return "+" + s + "\r\n";
}

std::string error(const std::string& msg) {
    return "-" + msg + "\r\n";
}

std::string integer(long long n) {
    return ":" + std::to_string(n) + "\r\n";
}

std::string bulk_string(const std::string& s) {
    return "$" + std::to_string(s.size()) + "\r\n" + s + "\r\n";
}

std::string null_bulk() {
    return "$-1\r\n";
}

ParseResult parse_command(const std::string& buf) {
    // TODO
    (void)buf;
    return ParseResult{};
}

}  // namespace resp
