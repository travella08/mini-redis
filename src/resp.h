#pragma once

#include <cstddef>
#include <string>
#include <vector>

// RESP: https://redis.io/docs/latest/develop/reference/protocol-spec/
namespace resp {

std::string simple_string(const std::string& s);  // +OK\r\n
std::string error(const std::string& msg);        // -ERR msg\r\n
std::string integer(long long n);                 // :42\r\n
std::string bulk_string(const std::string& s);    // $2\r\nhi\r\n
std::string null_bulk();                          // $-1\r\n

enum class ParseStatus { Ok, Incomplete, Error };

struct ParseResult {
    ParseStatus status = ParseStatus::Incomplete;
    std::vector<std::string> args;
    std::size_t consumed = 0;  // bytes read from the front of the buffer
    std::string error;
};

// Parses one command from the front of buf. Accepts RESP arrays of bulk
// strings and inline commands ("GET key\r\n").
ParseResult parse_command(const std::string& buf);

}  // namespace resp
