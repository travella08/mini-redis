// YOU WRITE THIS FILE. Milestone 1.
// Every function below is a stub that compiles but does nothing useful,
// so the m1 tests fail until you implement them.
#include "store.h"

void Store::set(const std::string& key, const std::string& value) {
    // TODO
    (void)key;
    (void)value;
}

std::optional<std::string> Store::get(const std::string& key) const {
    // TODO
    (void)key;
    return std::nullopt;
}

bool Store::del(const std::string& key) {
    // TODO
    (void)key;
    return false;
}

bool Store::exists(const std::string& key) const {
    // TODO
    (void)key;
    return false;
}

std::size_t Store::size() const {
    // TODO
    return 0;
}
