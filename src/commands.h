#pragma once

#include <string>
#include <vector>

#include "store.h"

// Runs a command (args[0] is the name, case-insensitive) and returns the
// RESP-encoded reply.
std::string execute(Store& store, const std::vector<std::string>& args);
