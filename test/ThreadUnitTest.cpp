/*
 * Copyright (C) 2013-2017 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common/Thread.hpp"

#include <gtest/gtest.h>

using audio_utilities::cme::common::Thread;

TEST(Thread, startStop)
{
    class TestThread : public Thread
    {
    public:
        TestThread(volatile bool &done) : Thread(""), _done(done) {}
        void processing() { _done = true; }
        volatile bool &_done;
    };

    volatile bool done = false;
    TestThread thr(done);
    EXPECT_TRUE(thr.start());
    while (!done) {
        usleep(100);
    }
    thr.stop();
}

TEST(Thread, stopRequested)
{
    class TestThread : public Thread
    {
    public:
        TestThread() : Thread("") { EXPECT_FALSE(isStopRequested()); }
        void processing() { usleep(10); }
        /* Have isStopRequested to be public */
        bool isStopRequested() { return Thread::isStopRequested(); }
    };

    TestThread thr;
    EXPECT_TRUE(thr.start());
    EXPECT_FALSE(thr.isStopRequested());
    usleep(100);
    thr.stop();
}

TEST(Thread, abort)
{
    class TestThread : public Thread
    {
    public:
        TestThread() : Thread("") { EXPECT_FALSE(isStopRequested()); }
        void processing() { selfAbort(); }
        /* Have isStopRequested to be public */
        bool isStopRequested() { return Thread::isStopRequested(); }
    };

    TestThread thr;
    EXPECT_FALSE(thr.isStopRequested());
    EXPECT_TRUE(thr.start());
    do {
        usleep(1000);
    } while (!thr.isStopRequested());
    thr.stop();
}

TEST(Thread, isStarted)
{
    class TestThread : public Thread
    {
    public:
        TestThread() : Thread("") {}
        void processing() { usleep(10); }
    };
    TestThread thr;
    EXPECT_FALSE(thr.isStarted());
    EXPECT_TRUE(thr.start());
    EXPECT_TRUE(thr.isStarted());
    usleep(100);
    EXPECT_TRUE(thr.isStarted());
    thr.stop();
    EXPECT_FALSE(thr.isStarted());
}

TEST(Thread, startAlreadyStarted)
{
    class TestThread : public Thread
    {
    public:
        TestThread() : Thread("") {}
        void processing() { usleep(100); }
    };

    TestThread thr;
    EXPECT_TRUE(thr.start());
    EXPECT_FALSE(thr.start());
    thr.stop();
}

TEST(Thread, respawnSameThread)
{
    class TestThread : public Thread
    {
    public:
        TestThread() : Thread("") {}
        void processing() { usleep(100); }
    };

    TestThread thr;
    EXPECT_TRUE(thr.start());
    thr.stop();
    EXPECT_TRUE(thr.start());
    thr.stop();
}

TEST(Thread, name)
{
    static const std::string mTestName = "TestThreadName";

    class TestThread : public Thread
    {
    public:
        TestThread() : Thread(mTestName) {}
        void processing()
        {
            mRetrievedName = getCurrentThreadName();
            selfAbort();
        }
        std::string mRetrievedName;
    };

    TestThread thr;
    EXPECT_TRUE(thr.start());
    thr.stop();
    EXPECT_EQ(mTestName, thr.mRetrievedName);
}
