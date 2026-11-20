// main.cpp: for now, a local REPL so you can poke at your store and commands
// before any networking exists. (Generated scaffolding.)
//
//   $ ./build/mini-redis
//   mini-redis> set name tanav
//   "+OK\r\n"
//
// In milestone 3 this file becomes the network server's entry point.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "commands.h"
#include "store.h"

namespace {

// Show \r and \n visibly so you can see the exact RESP bytes.
std::string escape(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '\r') out += "\\r";
        else if (c == '\n') out += "\\n";
        else out += c;
    }
    return "\"" + out + "\"";
}

}  // namespace

int main() {
    Store store;
    std::string line;

    std::cout << "mini-redis REPL (Ctrl+D to quit)\n";
    while (std::cout << "mini-redis> " << std::flush, std::getline(std::cin, line)) {
        std::istringstream words(line);
        std::vector<std::string> args;
        for (std::string w; words >> w;) args.push_back(w);
        if (args.empty()) continue;

        std::cout << escape(execute(store, args)) << "\n";
    }
    std::cout << "\n";
}
