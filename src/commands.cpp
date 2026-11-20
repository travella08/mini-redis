// YOU WRITE THIS FILE. Milestone 1.
//
// Suggested path:
//   1. Get it working with a plain if / else-if chain on the command name.
//   2. Once all m1 tests pass, refactor to a lookup table:
//      std::unordered_map<std::string, Handler>, where Handler is a function
//      that takes (Store&, args) and returns the reply. Ask Claude to review
//      your design before and after.
#include "commands.h"

#include "resp.h"

std::string execute(Store& store, const std::vector<std::string>& args) {
    // TODO
    (void)store;
    (void)args;
    return resp::error("ERR not implemented");
}
