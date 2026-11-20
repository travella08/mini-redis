# Roadmap

**Goal:** be able to explain every line of this in an interview.

**Who writes what**
- 🧑 **You write it.** Claude explains concepts, gives hints and reviews your code, but doesn't write it.
- 🤖 **Claude generates it.** Only things nobody asks about in interviews: build config, CI, test files, scripts, REPL.

Every milestone ends with **interview questions to check yourself**. Answer them out loud without looking at the code. If you can't, that's the part to revisit.

---

## M1: Store + commands (no networking)
- [ ] 🧑 `Store` in `src/store.cpp`
- [ ] 🧑 RESP encoders in `src/resp.cpp`
- [ ] 🧑 `execute()` in `src/commands.cpp`: PING, ECHO, SET, GET, DEL, EXISTS
- [ ] 🧑 Refactor the if/else chain into a command lookup table
- [x] 🤖 Tests, REPL, CMake, CI

**Done when:** `ctest --test-dir build -R m1` is all green.

**Interview questions to check yourself**
- Why does `get` return `std::optional` instead of an empty string?
- What's the time complexity of GET and SET, and why?
- How do you add a new command? Why is a table better than if/else?

## M2: RESP parser
- [ ] 🧑 `resp::parse_command` in `src/resp.cpp`
- [x] 🤖 Tests

**Done when:** `ctest --test-dir build -R m2` is all green.

**Interview questions to check yourself**
- Why can't you just split on `\r\n`?
- What does "TCP is a byte stream" mean, and how does your parser handle half a command?
- What is pipelining and why does it make Redis faster?

## M3: Networking
- [ ] 🧑 Blocking echo server: `socket` → `bind` → `listen` → `accept` → `read`/`write`
- [ ] 🧑 Blocking Redis server: one client at a time, using your parser + `execute()`
- [ ] 🧑 Event loop with `poll()`: non-blocking sockets, per-client input/output buffers
- [ ] 🤖 Integration test script using `redis-cli`

**Done when:** two `redis-cli` windows work against your server at the same time.

**Interview questions to check yourself**
- Walk through what happens from `redis-cli SET a b` to the `+OK` coming back.
- Why does a blocking server fail with two clients?
- How does `poll()` fix that? What does non-blocking mean for `read()`?
- Why is Redis single-threaded, and why is that fast instead of slow?
- What happens if a client disconnects in the middle of a reply? (Hint: SIGPIPE.)

## M4: Expiration
- [ ] 🧑 `EXPIRE`, `TTL`, `PERSIST`, `SET key value EX seconds`
- [ ] 🧑 Lazy expiry (check on access) + active expiry (sample keys periodically)
- [ ] 🤖 Tests

**Interview questions to check yourself**
- Why not a timer per key?
- What's the trade-off between lazy and active expiry?

## M5: Vector search, brute force ⭐
- [ ] 🧑 Second value type: the store now holds strings OR vector indexes
- [ ] 🧑 `VADD index id f1 f2 ...` and `VSEARCH index k f1 f2 ...` (cosine similarity)
- [ ] 🤖 Tests + script that loads real embeddings

**Interview questions to check yourself**
- What's an embedding and why does cosine similarity measure meaning?
- How did you represent "string or vector index" in C++? (`std::variant`?) Why?
- What's the cost of a brute-force query in terms of n and d?

## M6: Vector search, HNSW ⭐⭐
- [ ] 🧑 Hierarchical Navigable Small World index
- [ ] 🧑 Measure recall@k vs brute force, and speedup
- [ ] 🤖 Benchmark harness

**Interview questions to check yourself**
- Explain HNSW on a whiteboard: layers, greedy search, the `M` and `ef` parameters.
- Why is it approximate, and how did you measure how approximate?
- State your result: "X× faster than brute force at Y% recall on N vectors."

## M7: Polish for the resume
- [ ] 🤖 `redis-benchmark` script comparing against real Redis
- [ ] 🧑 README: architecture diagram, design decisions, benchmark numbers (write this yourself, it's interview prep)
- [ ] Optional: persistence (append-only file), LRU eviction with `--max-keys`
