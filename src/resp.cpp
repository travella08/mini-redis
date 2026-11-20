#include "resp.h"

namespace resp {

std::string simple_string(const std::string& s) {
    // TODO
    (void)s;
    return "";
}

std::string error(const std::string& msg) {
    // TODO
    (void)msg;
    return "";
}

std::string integer(long long n) {
    // TODO
    (void)n;
    return "";
}

std::string bulk_string(const std::string& s) {
    // TODO
    (void)s;
    return "";
}

std::string null_bulk() {
    // TODO
    return "";
}

ParseResult parse_command(const std::string& buf) {
    // TODO
    (void)buf;
    return ParseResult{};
}

}  // namespace resp
