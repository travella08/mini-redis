// Local REPL for testing commands until the server exists.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "commands.h"
#include "store.h"

namespace {

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

    while (std::cout << "mini-redis> " << std::flush, std::getline(std::cin, line)) {
        std::istringstream words(line);
        std::vector<std::string> args;
        for (std::string w; words >> w;) args.push_back(w);
        if (args.empty()) continue;

        std::cout << escape(execute(store, args)) << "\n";
    }
    std::cout << "\n";
}
