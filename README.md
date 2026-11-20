# mini-redis

A Redis-compatible in-memory database written from scratch in C++17, with built-in vector similarity search.

> 🚧 Work in progress. See [ROADMAP.md](ROADMAP.md).

## Build & test

Requires CMake ≥ 3.16 and a C++17 compiler (clang or gcc). macOS: `xcode-select --install && brew install cmake`.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure        # all tests
ctest --test-dir build -R m1 --output-on-failure  # just one milestone
```

## Try it

```bash
./build/mini-redis
mini-redis> SET name tanav
"+OK\r\n"
mini-redis> GET name
"$5\r\ntanav\r\n"
```

## Design

_To be written as the project grows: architecture, why single-threaded, how expiration works, how the vector index works._
