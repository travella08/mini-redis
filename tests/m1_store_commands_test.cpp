// Milestone 1 spec: Store, RESP encoders, and basic commands.
// Run: ctest --test-dir build -R m1 --output-on-failure
#include <gtest/gtest.h>

#include "commands.h"
#include "resp.h"
#include "store.h"

// ===== Store ===============================================================

TEST(Store, GetMissingKeyIsEmpty) {
    Store s;
    EXPECT_FALSE(s.get("nope").has_value());
}

TEST(Store, SetThenGet) {
    Store s;
    s.set("name", "tanav");
    ASSERT_TRUE(s.get("name").has_value());
    EXPECT_EQ(*s.get("name"), "tanav");
}

TEST(Store, SetOverwrites) {
    Store s;
    s.set("k", "first");
    s.set("k", "second");
    EXPECT_EQ(*s.get("k"), "second");
    EXPECT_EQ(s.size(), 1u);
}

TEST(Store, EmptyStringIsAValidValue) {
    // This is why get() returns optional instead of "" for missing keys.
    Store s;
    s.set("k", "");
    ASSERT_TRUE(s.get("k").has_value());
    EXPECT_EQ(*s.get("k"), "");
}

TEST(Store, DelReportsWhetherKeyExisted) {
    Store s;
    s.set("k", "v");
    EXPECT_TRUE(s.del("k"));
    EXPECT_FALSE(s.del("k"));
    EXPECT_FALSE(s.exists("k"));
    EXPECT_EQ(s.size(), 0u);
}

TEST(Store, Exists) {
    Store s;
    EXPECT_FALSE(s.exists("k"));
    s.set("k", "v");
    EXPECT_TRUE(s.exists("k"));
}

// ===== RESP encoders =======================================================

TEST(Encode, SimpleString) { EXPECT_EQ(resp::simple_string("OK"), "+OK\r\n"); }
TEST(Encode, Error) { EXPECT_EQ(resp::error("ERR bad"), "-ERR bad\r\n"); }
TEST(Encode, Integer) {
    EXPECT_EQ(resp::integer(0), ":0\r\n");
    EXPECT_EQ(resp::integer(42), ":42\r\n");
    EXPECT_EQ(resp::integer(-7), ":-7\r\n");
}
TEST(Encode, BulkString) {
    EXPECT_EQ(resp::bulk_string("tanav"), "$5\r\ntanav\r\n");
    EXPECT_EQ(resp::bulk_string(""), "$0\r\n\r\n");
}
TEST(Encode, NullBulk) { EXPECT_EQ(resp::null_bulk(), "$-1\r\n"); }

// ===== Commands ============================================================

class Commands : public ::testing::Test {
protected:
    Store store;
    std::string run(const std::vector<std::string>& args) { return execute(store, args); }
    static bool is_error(const std::string& reply) {
        return reply.rfind("-ERR", 0) == 0 && reply.size() >= 2 &&
               reply.compare(reply.size() - 2, 2, "\r\n") == 0;
    }
};

TEST_F(Commands, Ping) {
    EXPECT_EQ(run({"PING"}), "+PONG\r\n");
    EXPECT_EQ(run({"PING", "hello"}), "$5\r\nhello\r\n");
}

TEST_F(Commands, Echo) { EXPECT_EQ(run({"ECHO", "hi"}), "$2\r\nhi\r\n"); }

TEST_F(Commands, SetGet) {
    EXPECT_EQ(run({"SET", "name", "tanav"}), "+OK\r\n");
    EXPECT_EQ(run({"GET", "name"}), "$5\r\ntanav\r\n");
}

TEST_F(Commands, GetMissing) { EXPECT_EQ(run({"GET", "nope"}), "$-1\r\n"); }

TEST_F(Commands, CaseInsensitiveNames) {
    EXPECT_EQ(run({"set", "k", "v"}), "+OK\r\n");
    EXPECT_EQ(run({"Get", "k"}), "$1\r\nv\r\n");
    EXPECT_EQ(run({"pInG"}), "+PONG\r\n");
}

TEST_F(Commands, KeysAreCaseSensitive) {
    run({"SET", "Key", "v"});
    EXPECT_EQ(run({"GET", "key"}), "$-1\r\n");
}

TEST_F(Commands, DelCountsRemovedKeys) {
    run({"SET", "a", "1"});
    run({"SET", "b", "2"});
    EXPECT_EQ(run({"DEL", "a", "b", "c"}), ":2\r\n");
    EXPECT_EQ(run({"GET", "a"}), "$-1\r\n");
}

TEST_F(Commands, ExistsCountsKeys) {
    run({"SET", "a", "1"});
    EXPECT_EQ(run({"EXISTS", "a"}), ":1\r\n");
    EXPECT_EQ(run({"EXISTS", "a", "a", "zzz"}), ":2\r\n");  // real Redis counts repeats
}

TEST_F(Commands, UnknownCommandIsError) {
    EXPECT_TRUE(is_error(run({"FLY", "away"})));
}

TEST_F(Commands, WrongArgCountIsError) {
    EXPECT_TRUE(is_error(run({"GET"})));
    EXPECT_TRUE(is_error(run({"GET", "a", "b"})));
    EXPECT_TRUE(is_error(run({"SET", "onlykey"})));
    EXPECT_TRUE(is_error(run({"DEL"})));
    EXPECT_TRUE(is_error(run({"ECHO"})));
}

TEST_F(Commands, EmptyCommandIsError) { EXPECT_TRUE(is_error(run({}))); }
