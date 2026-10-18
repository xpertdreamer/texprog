#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "tdsqueue.h"

TEST_CASE("testing queue pushing") {
    TdSQueue<int> q;
    q.push(12);
    q.push(53);
    CHECK(q.pop() == 12);
    CHECK(q.pop() == 53);
}

TEST_CASE("testing queue empty") {
    TdSQueue<int> q;
    q.push(12);
    CHECK(q.empty() == false);
    q.pop();
    CHECK(q.empty() == true);
}

TEST_CASE("testing queue size") {
    TdSQueue<int> q;
    q.push(12);
    CHECK(q.size() == 1);
    q.pop();
    CHECK(q.size() == 0);
}

TEST_CASE("testing queue pop") {
    TdSQueue<int> q;
    auto p = q.pop();
    CHECK(p == 0);
}
