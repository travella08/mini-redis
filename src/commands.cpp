#include "commands.h"

#include <cctype>

#include "resp.h"

namespace {

std::string wrong_args(const std::string& name) {
    return resp::error("ERR wrong number of arguments for '" + name + "' command");
}

}  // namespace

std::string execute(Store& store, const std::vector<std::string>& args) {
    if (args.empty()) {
        return resp::error("ERR empty command");
    }
    std::string command = args[0];
    for (char& c : command) {
        c = std::toupper(static_cast<unsigned char>(c));
    }

    if (command == "PING") {
        if (args.size() == 1) {
            return resp::simple_string("PONG");
        }
        if (args.size() == 2) {
            return resp::bulk_string(args[1]);
        }
        return wrong_args(args[0]);
    }
    if (command == "ECHO") {

        if (args.size() != 2) {
            return wrong_args(args[0]);
        }
        return resp::bulk_string(args[1]);
    }
    if (command == "SET") {
        if (args.size() != 3) {
            return wrong_args(args[0]);
        }
        store.set(args[1], args[2]);
        return resp::simple_string("OK");
    }
    if (command == "GET") {
        if (args.size() != 2) {
            return wrong_args(args[0]);
        }
        auto value = store.get(args[1]);
        if (value) {
            return resp::bulk_string(*value);
        }
        return resp::null_bulk();
    }
    if (command == "DEL") {
        if (args.size() < 2) {
            return wrong_args(args[0]);
        }
        long long removed = 0;
        for (size_t i = 1; i < args.size(); i++) {
            if (store.del(args[i])) {
                removed++;
            }
        }
        return resp::integer(removed);
    }
    if (command == "EXISTS") {
        if (args.size() < 2) {
            return wrong_args(args[0]);
        }
        long long found = 0;
        for (size_t i = 1; i < args.size(); i++) {
            if (store.exists(args[i])) {
                found++;
            }
        }
        return resp::integer(found);
    }

    return resp::error("ERR unknown command '" + args[0] + "'");
}
