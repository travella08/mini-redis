// resp.h: RESP, the REdis Serialization Protocol.
// Spec: https://redis.io/docs/latest/develop/reference/protocol-spec/
//
// YOU WRITE: resp.cpp
//   Milestone 1: the encoders (they turn values into reply bytes).
//   Milestone 2: parse_command (turns incoming bytes into a command).
#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace resp {

// ---- Encoders (milestone 1) -----------------------------------------------
std::string simple_string(const std::string& s);  // "OK"    -> "+OK\r\n"
std::string error(const std::string& msg);        // "ERR x" -> "-ERR x\r\n"
std::string integer(long long n);                 // 42      -> ":42\r\n"
std::string bulk_string(const std::string& s);    // "hi"    -> "$2\r\nhi\r\n"
std::string null_bulk();                          //         -> "$-1\r\n"

// ---- Parser (milestone 2) -------------------------------------------------
enum class ParseStatus {
    Ok,          // one full command parsed
    Incomplete,  // need more bytes, try again after the next read()
    Error,       // the bytes are not valid RESP
};

struct ParseResult {
    ParseStatus status = ParseStatus::Incomplete;
    std::vector<std::string> args;  // e.g. {"SET", "name", "tanav"}  (when Ok)
    std::size_t consumed = 0;       // bytes used from the front of buf (when Ok)
    std::string error;              // human-readable reason (when Error)
};

// Parse ONE command from the front of `buf`. Must handle:
//   - RESP arrays of bulk strings:  "*2\r\n$3\r\nGET\r\n$4\r\nname\r\n"
//   - inline commands (from nc):    "GET name\r\n"
// `buf` may hold a partial command, or several commands back to back.
ParseResult parse_command(const std::string& buf);

}  // namespace resp
