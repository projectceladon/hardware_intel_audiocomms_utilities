/*
 ********************************************************************************
 *                              INTEL CONFIDENTIAL
 *   Copyright(C) 2013-2014 Intel Corporation. All Rights Reserved.
 *   The source code contained  or  described herein and all documents related to
 *   the source code ("Material") are owned by Intel Corporation or its suppliers
 *   or licensors.  Title to the  Material remains with  Intel Corporation or its
 *   suppliers and licensors. The Material contains trade secrets and proprietary
 *   and  confidential  information of  Intel or its suppliers and licensors. The
 *   Material  is  protected  by  worldwide  copyright  and trade secret laws and
 *   treaty  provisions. No part of the Material may be used, copied, reproduced,
 *   modified, published, uploaded, posted, transmitted, distributed or disclosed
 *   in any way without Intel's prior express written permission.
 *   No license  under any  patent, copyright, trade secret or other intellectual
 *   property right is granted to or conferred upon you by disclosure or delivery
 *   of the Materials,  either expressly, by implication, inducement, estoppel or
 *   otherwise.  Any  license  under  such  intellectual property  rights must be
 *   express and approved by Intel in writing.
 *
 ********************************************************************************
 */

#pragma once

#include <NonCopyable.hpp>
#include <AudioCommsAssert.hpp>

#include <pthread.h>

namespace audio_comms
{
namespace cme
{
namespace common
{

/**
 * Helper class to spawn and handle a thread
 *
 * This class is NOT thread safe.
 * The methods start, stop, isStarted and the destructor
 * MUST not be called simultaneously.
 */
class Thread : private audio_comms::utilities::NonCopyable
{
public:
    /** Create a thread with a name.
     * @param[in] name The name that the thread should give himself.
     *                 "" for default (inherit parent name).
     *                 If longer than MAX_TASK_COMM_LEN, will be truncated.
     */
    Thread(const std::string name)
        : _stopRequested(false), _thread(0), mName(name)
    {
        if (mName.length() > MAX_TASK_COMM_LEN) {
            mName.resize(MAX_TASK_COMM_LEN);
        }
    }

    virtual ~Thread()
    {
        AUDIOCOMMS_ASSERT(_thread == 0, "Can not destroy a running thread");
    }

    /**
     * Start the thread.
     * Calling this function on an already started thread will return false.
     *
     * @return true on success false otherwise
     */
    virtual bool start();

    /** Ask for the tread to stop and waits until it is.
     * Note: this is a collaborative stop, which means that if the processing
     * function is blocking, the thread will stop on processing completion.
     * This is user responsibility to prevent deadlocks in processing.
     */
    virtual void stop();

    /**
     * @return true is the thread has been started
     */
    bool isStarted() const { return _thread != 0; }

protected:
    /** Tells if the thread is requested to stop
     * Note: This does NOT mean that the thread is stopped. The thread is
     * guaranteed to be stopped only when stop() returns.
     * @return true if the thread was requested to stop.*/
    bool isStopRequested() const { return _stopRequested; }

    /**
     * This method allows the Thread to terminate itself.
     * The subclasse can call this method from its processing() method in order
     * to abort its execution. Once selfAbort() is called, the processing() will
     * never be recalled as soon as it returns.
     * The Thread user can safely call the stop() method while the thread has aborted itself.
     */
    void selfAbort();

    /** Max thread name length.
     *
     *  16 with the terminating null char.
     *  @note: Not exposed by kernel headers
     */
    static const size_t MAX_TASK_COMM_LEN = 15;

private:
    /** The processing function which will repeatedly be called by the thread.
     *
     * It will be called at least once.
     */
    virtual void processing() = 0;

    /**
     * This method can be overloaded to perform shutdown stuff
     */
    virtual void shutdown() {}

    void loop()
    {
        setDebugName();
        do {
            processing();
        } while (!_stopRequested);
    }

    /** Set the current thread name to ease debug.
     *
     * Must not be longer than MAX_TASK_COMM_LEN characters or it will be
     * truncated.
     */
    bool setDebugName();

    static void *threadMainHelper(void *);

    volatile bool _stopRequested; /**< Should the thread keep running? */
    pthread_t _thread;           /**< the thread handle */
    std::string mName;
};

} // namespace audio_comms
} // namespace cme
} // namespace common
