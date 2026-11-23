#include "commands.h"

#include "resp.h"

std::string execute(Store& store, const std::vector<std::string>& args) {
    std::unordered_map<std::string, std::string> data_;

    (void)store;
    (void)args;
    return resp::error("ERR not implemented");
}
