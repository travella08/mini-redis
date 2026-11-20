// store.h: the in-memory key-value store.
//
// YOU WRITE: store.cpp
// The interface below is what the tests expect. You may add private members
// and helper functions, but keep these public signatures.
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>

class Store {
public:
    // Insert or overwrite.
    void set(const std::string& key, const std::string& value);

    // The value, or std::nullopt if the key doesn't exist.
    std::optional<std::string> get(const std::string& key) const;

    // Remove a key. Returns true if it existed.
    bool del(const std::string& key);

    bool exists(const std::string& key) const;

    // Number of keys currently stored.
    std::size_t size() const;

private:
    std::unordered_map<std::string, std::string> data_;
};
