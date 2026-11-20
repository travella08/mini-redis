// Milestone 2 spec: the RESP parser.
// Run: ctest --test-dir build -R m2 --output-on-failure
#include <gtest/gtest.h>

#include "resp.h"

using resp::ParseStatus;
using Args = std::vector<std::string>;

TEST(Parse, EmptyBufferIsIncomplete) {
    EXPECT_EQ(resp::parse_command("").status, ParseStatus::Incomplete);
}

TEST(Parse, FullArray) {
    const std::string buf = "*2\r\n$3\r\nGET\r\n$4\r\nname\r\n";
    auto r = resp::parse_command(buf);
    ASSERT_EQ(r.status, ParseStatus::Ok);
    EXPECT_EQ(r.args, (Args{"GET", "name"}));
    EXPECT_EQ(r.consumed, buf.size());
}

TEST(Parse, EveryPrefixOfACommandIsIncomplete) {
    // TCP can split a message anywhere. Your parser must cope with all of them.
    const std::string buf = "*3\r\n$3\r\nSET\r\n$4\r\nname\r\n$5\r\ntanav\r\n";
    for (size_t len = 0; len < buf.size(); ++len) {
        EXPECT_EQ(resp::parse_command(buf.substr(0, len)).status, ParseStatus::Incomplete)
            << "prefix of length " << len << " should be Incomplete";
    }
    EXPECT_EQ(resp::parse_command(buf).status, ParseStatus::Ok);
}

TEST(Parse, OnlyConsumesTheFirstCommand) {
    // Pipelining: two commands arrive in one read().
    const std::string first = "*1\r\n$4\r\nPING\r\n";
    const std::string second = "*2\r\n$4\r\nECHO\r\n$2\r\nhi\r\n";
    auto r = resp::parse_command(first + second);
    ASSERT_EQ(r.status, ParseStatus::Ok);
    EXPECT_EQ(r.args, (Args{"PING"}));
    EXPECT_EQ(r.consumed, first.size());

    auto r2 = resp::parse_command((first + second).substr(r.consumed));
    ASSERT_EQ(r2.status, ParseStatus::Ok);
    EXPECT_EQ(r2.args, (Args{"ECHO", "hi"}));
}

TEST(Parse, BulkStringsAreBinarySafe) {
    // The length prefix means values can contain \r\n themselves.
    auto r = resp::parse_command("*2\r\n$4\r\nECHO\r\n$4\r\na\r\nb\r\n");
    ASSERT_EQ(r.status, ParseStatus::Ok);
    EXPECT_EQ(r.args, (Args{"ECHO", "a\r\nb"}));
}

TEST(Parse, EmptyBulkString) {
    auto r = resp::parse_command("*3\r\n$3\r\nSET\r\n$1\r\nk\r\n$0\r\n\r\n");
    ASSERT_EQ(r.status, ParseStatus::Ok);
    EXPECT_EQ(r.args, (Args{"SET", "k", ""}));
}

TEST(Parse, InlineCommand) {
    auto r = resp::parse_command("SET name   tanav\r\nGET name\r\n");
    ASSERT_EQ(r.status, ParseStatus::Ok);
    EXPECT_EQ(r.args, (Args{"SET", "name", "tanav"}));
    EXPECT_EQ(r.consumed, 18u);
}

TEST(Parse, InlineWithoutNewlineIsIncomplete) {
    EXPECT_EQ(resp::parse_command("PING").status, ParseStatus::Incomplete);
}

TEST(Parse, BadArrayLengthIsError) {
    EXPECT_EQ(resp::parse_command("*abc\r\n").status, ParseStatus::Error);
    EXPECT_EQ(resp::parse_command("*-5\r\n").status, ParseStatus::Error);
}

TEST(Parse, MissingDollarIsError) {
    EXPECT_EQ(resp::parse_command("*1\r\n+PING\r\n").status, ParseStatus::Error);
}

TEST(Parse, BadBulkLengthIsError) {
    EXPECT_EQ(resp::parse_command("*1\r\n$x\r\nPING\r\n").status, ParseStatus::Error);
}

TEST(Parse, BulkNotTerminatedByCrlfIsError) {
    // Says 4 bytes, but the 4 bytes are followed by junk instead of \r\n.
    EXPECT_EQ(resp::parse_command("*1\r\n$4\r\nPINGxx").status, ParseStatus::Error);
}
