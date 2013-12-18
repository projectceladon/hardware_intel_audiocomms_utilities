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

class RemoteParameterConnector;
class IRemoteParameter;

/**
 * Remote Parameter (Server Side) implementation based on android socket.
 * It makes use of a custom protocol to set / get parameters.
 *
 * First, the command identifier is received as a size.
 *          0 -> Get command
 *          non null -> Set command with the size of the parameter to set
 *
 * Then, for a Get command:
 *              Server Side Remote Parameter implementor sends the size of the parameter.
 *              Server Side Remote Parameter implementor sends the value of the parameter.
 *
 *       for a set command:
 *              Server Side Remote Parameter implementor receives the value of the parameter to set.
 *              Server Side Remote Parameter callback the set function of the interface.
 *              Server Side Remote Parameter sends the status to the client.
 */
class RemoteParameterImpl
{
public:
    RemoteParameterImpl(IRemoteParameter *interface,
                        const std::string &parameterName,
                        uint32_t size);

    ~RemoteParameterImpl();

    /**
     * Client connection handler
     * When the Event Thread is woken up on an event of the remote parameter.
     */
    void handleNewConnection();

    /**
     * Return a file descriptor to poll.
     * Requested by EventListener based implementation of the server.
     *
     * @return file descriptor of the remote parameter (server side) to poll.
     */
    int getPollFd();

    /**
     * Get the name of the parameter.
     *
     * @return name of the parameter.
     */
    const std::string getName() const { return _name; }

private:

    std::string _name; /**< Parameter Name. */
    uint32_t _size; /**< Parameter Size. */

    IRemoteParameter *_interface; /**< Interface for set/get operation. */

    RemoteParameterConnector *_serverConnector; /**< Remote Parameter Server Side connector. */

    static const uint32_t _communicationTimeoutMs = 5000; /**< Timeout. */
};
