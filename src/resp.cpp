// YOU WRITE THIS FILE.
//   Milestone 1: the five encoders.
//   Milestone 2: parse_command.
#include "resp.h"

namespace resp {

std::string simple_string(const std::string& s) {
    // TODO (m1)
    (void)s;
    return "";
}

std::string error(const std::string& msg) {
    // TODO (m1)
    (void)msg;
    return "";
}

std::string integer(long long n) {
    // TODO (m1)
    (void)n;
    return "";
}

std::string bulk_string(const std::string& s) {
    // TODO (m1)
    (void)s;
    return "";
}

std::string null_bulk() {
    // TODO (m1)
    return "";
}

ParseResult parse_command(const std::string& buf) {
    // TODO (m2). Hints for when you get here:
    //   - Look at buf[0]: '*' means a RESP array, anything else means inline.
    //   - std::string::find("\r\n", pos) finds the end of a line.
    //   - Before reading N bytes, check that N bytes (plus "\r\n") are
    //     actually in the buffer. If not, the answer is Incomplete, not Error.
    (void)buf;
    return ParseResult{};
}

}  // namespace resp
