// commands.h: turns a parsed command into a RESP reply.
//
// YOU WRITE: commands.cpp
#pragma once

#include <string>
#include <vector>

#include "store.h"

// Run one command against the store and return the encoded reply.
//   args[0] is the command name (case-insensitive: "set", "SET", "Set").
//   The rest are its arguments.
//
// Milestone 1 commands:
//   PING [message]      -> +PONG, or the message as a bulk string
//   ECHO message        -> message as a bulk string
//   SET key value       -> +OK
//   GET key             -> bulk string, or null bulk if missing
//   DEL key [key ...]   -> integer: how many keys were removed
//   EXISTS key [key...] -> integer: how many of the keys exist
//
// Errors (unknown command, wrong number of args) return a RESP error
// starting with "ERR". Never throw, never crash.
std::string execute(Store& store, const std::vector<std::string>& args);
