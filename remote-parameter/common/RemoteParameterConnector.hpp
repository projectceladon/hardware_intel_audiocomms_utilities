/*
 * Copyright 2014 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <NonCopyable.hpp>
#include <string>
#include <stdint.h>
#include <sys/types.h>

class RemoteParameterConnector : private audio_comms::utilities::NonCopyable
{
public:
    static const uint32_t mCommunicationTimeoutMs = 5000; /**< Timeout. */

    static const uint32_t mTransactionSucessfull = 0; /**< success status of a transaction. */
    static const uint32_t mTransactionFailed = -1; /**< failure status of a transaction. */

    static const uint32_t mSizeCommandGet = 0;  /**< get command identiable by its null size. */

    RemoteParameterConnector(int socketFd);
    ~RemoteParameterConnector();

    /**
     * Socket Fd
     */
    int getFd() const;

    /**
     * Checks if the remote parameter is connected, i.e. the socket is opened
     *
     * @return true if connected, false otherwise
     */
    bool isConnected() const;

    /**
     * Build server name from parameter one. It appends the string "parameter." to the parameter.
     *
     * @param[in] parameterName name of the parameter
     *
     * @return server name to use
     */
    static std::string getServerName(const std::string &parameterName);

    /**
     * Set the timeout for the transaction.
     *
     * @param[in] timeoutMs: timeout value in milliseconds.
     */
    void setTimeoutMs(uint32_t timeoutMs);

    /**
     * Send
     *
     * @param[in] data buffer to send through the connector
     * @param[in] size of the data to send in bytes.
     *
     * @return true if send is successful, false otherwise.
     */
    bool send(const void *data, uint32_t size);

    /**
     * Receive
     *
     * @param[out] data buffer to receive from the connector
     * @param[in] size of the data to receive in bytes.
     *
     * @return true if receive is successful, false otherwise.
     */
    bool receive(void *data, uint32_t size);

    /**
     * Get User ID.
     *
     * @return linux user id for file descriptor.
     */
    uid_t getUid() const;

    /**
     * Accept a connection on a socked.
     *
     * @return file descriptor of the client connected to the socket.
     */
    int acceptConnection();

private:
    int mSocketFd; /**< Socket ID (server or client). */

    static const char *const mBaseName; /**< Base string name of a parameter. */

    static const uint32_t mMsecPerSec = 1000; /**< milliseconds per second. */
    static const uint32_t mUsecPerMsec = 1000; /**< microseconds per milliseconds. */
};
