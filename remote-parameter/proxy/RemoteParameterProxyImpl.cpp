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

#define LOG_TAG "RemoteParameterProxyImpl"

#include "RemoteParameterProxyImpl.hpp"

#include <RemoteParameterConnector.hpp>
#include <AudioCommsAssert.hpp>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utils/Log.h>
#include <cutils/sockets.h>

using std::string;

const std::string RemoteParameterProxyImpl::_connectionError = "Not connected";

const std::string RemoteParameterProxyImpl::_sendSizeProtocolError = "Send Size:Protocol error";

const std::string RemoteParameterProxyImpl::_sendDataProtocolError = "Send Data:Protocol error";

const std::string RemoteParameterProxyImpl::_receiveProtocolError = "Receive:Protocol error";

const std::string RemoteParameterProxyImpl::_transactionRefusedError = "Transaction refused";

const std::string RemoteParameterProxyImpl::_getCommandError =
    "Protocol error: get should start with empty size";

const std::string RemoteParameterProxyImpl::_readSizeProtocolError =
    "Protocol error while trying to read the size";

const std::string RemoteParameterProxyImpl::_readDataProtocolError =
    "Protocol error: answered size does not meet the expected data size";

const std::string RemoteParameterProxyImpl::_readProtocolError =
    "Protocol error in receiving the data";

/**
 * Remote Parameter (Proxy Side) implementation based on socket
 *
 */
RemoteParameterProxyImpl::RemoteParameterProxyImpl(const string &parameterName,
                                                   uint32_t size)
    : _name(parameterName),
      _size(size)
{
}

RemoteParameterProxyImpl::~RemoteParameterProxyImpl()
{
}

bool RemoteParameterProxyImpl::write(const uint8_t *data, uint32_t size, string &error)
{
    AUDIOCOMMS_ASSERT(size == _size, "write called with wrong size");
    AUDIOCOMMS_ASSERT(data != NULL, "NULL data pointer");

    RemoteParameterConnector connector(socket_local_client(
                                           RemoteParameterConnector::getServerName(_name).c_str(),
                                           ANDROID_SOCKET_NAMESPACE_ABSTRACT,
                                           SOCK_STREAM));
    if (!connector.isConnected()) {

        error = _connectionError;
        return false;
    }

    // Set timeout
    connector.setTimeoutMs(_communicationTimeoutMs);

    /// For a set command first
    // By protocol convention, set command is identified with non-zero size sent first.
    // The size sent is the size of the parameter to set.
    if (!connector.send(static_cast<const void *>(&size), sizeof(size))) {

        error = _sendSizeProtocolError;
        return false;
    }

    /// Send data
    if (!connector.send((const void *)data, size)) {

        error = _sendDataProtocolError;
        return false;
    }

    /// Read answer (status)
    uint32_t status;

    // Status
    if (!connector.receive(static_cast<void *>(&status), sizeof(status))) {

        error = _receiveProtocolError;
        return false;
    }

    if (status != RemoteParameterConnector::_transactionSucessfull) {

        // Got an error
        error = _transactionRefusedError;
        return false;
    }

    return true;
}

bool RemoteParameterProxyImpl::read(uint8_t *data, uint32_t size, string &error)
{
    AUDIOCOMMS_ASSERT(size == _size, "Read called with wrong size");
    AUDIOCOMMS_ASSERT(data != NULL, "NULL data pointer");

    RemoteParameterConnector connector(socket_local_client(
                                           RemoteParameterConnector::getServerName(_name).c_str(),
                                           ANDROID_SOCKET_NAMESPACE_ABSTRACT,
                                           SOCK_STREAM));
    // Set timeout
    connector.setTimeoutMs(_communicationTimeoutMs);

    // Check connection
    if (!connector.isConnected()) {

        error = _connectionError;
        return false;
    }

    /// Send Get Command
    // By protocol convention, get command is identified by a NULL size
    uint32_t cmdSize = RemoteParameterConnector::_sizeCommandGet;
    if (!connector.send(static_cast<const void *>(&cmdSize), sizeof(cmdSize))) {

        error = _getCommandError;
        return false;
    }

    /// Read answer (status + data)
    uint32_t answerSize;

    // Size
    if (!connector.receive(static_cast<void *>(&answerSize), sizeof(answerSize))) {

        error = _readSizeProtocolError;
        return false;
    }

    if (answerSize != size) {

        error = _readDataProtocolError;
        return false;
    }

    // Read data
    if (!connector.receive(static_cast<void *>(data), size)) {

        error = _readProtocolError;
        return false;
    }

    return true;
}
