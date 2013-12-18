/*
 * INTEL CONFIDENTIAL
 *
 * Copyright 2013 Intel Corporation
 * All Rights Reserved.
 *
 * The source code contained or described herein and all documents related
 * to the source code ("Material") are owned by Intel Corporation or its
 * suppliers or licensors.
 * Title to the Material remains with Intel Corporation or its suppliers and
 * licensors. The Material contains trade secrets and proprietary and
 * confidential information of Intel or its suppliers and licensors.
 * The Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 */

#include "common/Thread.hpp"

#include <gtest/gtest.h>

using audio_comms::cme::common::Thread;

TEST(Thread, startStop)
{
    class TestThread : public Thread
    {
    public:
        TestThread() : Thread(), _count(0) {}
        void processing() { _count++; }
        volatile int _count;
    };

    TestThread thr;
    EXPECT_TRUE(thr.start());
    usleep(100);
    thr.stop();
    EXPECT_GT(thr._count, 1);
}

TEST(Thread, stopRequested)
{
    class TestThread : public Thread
    {
    public:
        TestThread() : Thread() { EXPECT_FALSE(isStopRequested()); }
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
        TestThread() : Thread() { EXPECT_FALSE(isStopRequested()); }
        void processing() { selfAbort(); }
        /* Have isStopRequested to be public */
        bool isStopRequested() { return Thread::isStopRequested(); }
    };

    TestThread thr;
    EXPECT_FALSE(thr.isStopRequested());
    EXPECT_TRUE(thr.start());
    usleep(1000);
    EXPECT_TRUE(thr.isStopRequested());
    thr.stop();
}

TEST(Thread, isStarted)
{
    class TestThread : public Thread
    {
    public:
        TestThread() : Thread() {}
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
        TestThread() : Thread() {}
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
        TestThread() : Thread() {}
        void processing() { usleep(100); }
    };

    TestThread thr;
    EXPECT_TRUE(thr.start());
    thr.stop();
    EXPECT_TRUE(thr.start());
    thr.stop();
}
