# mini-redis

A Redis-compatible in-memory key-value store written in C++17. WIP (last update 11/23/26)

## Prerequisites

- CMake 3.16+
- A C++17 compiler (clang or gcc)

## Build

```bash
git clone https://github.com/travella08/mini-redis.git
cd mini-redis
cmake -S . -B build
cmake --build build
```

## Usage

```bash
./build/mini-redis
mini-redis> SET name tanav
"+OK\r\n"
mini-redis> GET name
"$5\r\ntanav\r\n"
```

## Tests

```bash
ctest --test-dir build --output-on-failure
```
