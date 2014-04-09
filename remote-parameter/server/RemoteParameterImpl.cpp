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

#define LOG_TAG "RemoteParameter"

#include "RemoteParameterImpl.hpp"
#include "include/RemoteParameter.hpp"
#include <RemoteParameterConnector.hpp>
#include <AudioCommsAssert.hpp>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utils/Log.h>
#include <cutils/sockets.h>


using std::string;

/**
 * Remote Parameter (Server Side) implementation based on socket
 *
 */
RemoteParameterImpl::RemoteParameterImpl(RemoteParameterBase *parameter,
                                         const string &parameterName,
                                         size_t size)
    : _name(parameterName),
      _size(size),
      _parameter(parameter),
      _serverConnector(new RemoteParameterConnector(
                           socket_local_server(
                               RemoteParameterConnector::getServerName(_name).c_str(),
                               ANDROID_SOCKET_NAMESPACE_ABSTRACT,
                               SOCK_STREAM)))
{

}

int RemoteParameterImpl::getPollFd()
{
    AUDIOCOMMS_ASSERT(_serverConnector != NULL, "server connector invalid");

    return _serverConnector->getFd();
}

void RemoteParameterImpl::handleNewConnection()
{
    int clientSocketFd = _serverConnector->acceptConnection();
    if (clientSocketFd < 0) {

        ALOGE("%s: accept: %s", __FUNCTION__, strerror(errno));
        return;
    }

    RemoteParameterConnector clientConnector(clientSocketFd);

    // For the security check, compare client group id and server group id
    if (clientConnector.getGid() != _serverConnector->getGid()) {

        ALOGE("%s: security error", __FUNCTION__);
        return;
    }

    // Set timeout
    clientConnector.setTimeoutMs(_communicationTimeoutMs);

    /// Read command first
    // By protocol convention, get is 0, set is non null (size of the parameter to set in fact)
    uint32_t size;
    if (!clientConnector.receive((void *)&size, sizeof(size))) {

        ALOGE("%s: recv size: %s", __FUNCTION__, strerror(errno));
        return;
    }

    if (size == RemoteParameterConnector::_sizeCommandGet) {

        /// Get parameter
        size = _size;

        // Read data
        uint8_t data[size];

        _parameter->get(data, size);

        // Send Size
        if (!clientConnector.send((const void *)&size, sizeof(size))) {

            ALOGE("%s: send status: %s", __FUNCTION__, strerror(errno));
            return;
        }

        // Send data
        if (!clientConnector.send((const void *)data, size)) {

            ALOGE("%s: send data: %s", __FUNCTION__, strerror(errno));
        }
    } else {

        // Read data
        uint8_t data[size];

        if (!clientConnector.receive((void *)data, size)) {

            ALOGE("%s: recv data: %s", __FUNCTION__, strerror(errno));
            return;
        }

        // Set data
        uint32_t status = RemoteParameterConnector::_transactionSucessfull;
        if (!_parameter->set(data, size)) {

            status = RemoteParameterConnector::_transactionFailed;
        }

        // Send status back
        if (!clientConnector.send((const void *)&status, sizeof(status))) {

            ALOGE("%s: send status: %s", __FUNCTION__, strerror(errno));
        }
    }

}

RemoteParameterImpl::~RemoteParameterImpl()
{
    delete _serverConnector;
}
