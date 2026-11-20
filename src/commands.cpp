#include "commands.h"

#include "resp.h"

std::string execute(Store& store, const std::vector<std::string>& args) {
    // TODO
    (void)store;
    (void)args;
    return resp::error("ERR not implemented");
}
