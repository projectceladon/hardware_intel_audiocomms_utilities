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

#include <string>
#include <stdint.h>
#include <sys/types.h>

class RemoteParameterConnector
{
public:
    static const uint32_t _communicationTimeoutMs = 5000; /**< Timeout. */

    static const uint32_t _transactionSucessfull = 0; /**< success status of a transaction. */
    static const uint32_t _transactionFailed = -1; /**< failure status of a transaction. */

    static const uint32_t _sizeCommandGet = 0;  /**< get command identiable by its null size. */

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
     * Get Group ID.
     *
     * @eturn linux group id for file descriptor.
     */
    gid_t getGid();

    /**
     * Accept a connection on a socked.
     *
     * @return file descriptor of the client connected to the socket.
     */
    int acceptConnection();

private:
    std::string _parameterName; /**< Parameter Name. */
    uint32_t _size; /**< Parameter Size. */
    int _socketFd; /**< Socket ID (server or client). */

    static const std::string _baseName; /**< Base string name of a parameter. */

    static const uint32_t _msecPerSec = 1000; /**< milliseconds per second. */
    static const uint32_t _usecPerMsec = 1000; /**< microseconds per milliseconds. */
};
