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

#define LOG_TAG "RemoteParameterConnector"

#include "RemoteParameterConnector.hpp"
#include <AudioCommsAssert.hpp>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utils/Log.h>
#include <cutils/sockets.h>

const std::string RemoteParameterConnector::_baseName = "parameter.";

RemoteParameterConnector::RemoteParameterConnector(int socketFd)
    : _socketFd(socketFd)
{
}

RemoteParameterConnector::~RemoteParameterConnector()
{
    close(_socketFd);
}

bool RemoteParameterConnector::isConnected() const
{
    return _socketFd != -1;
}

std::string RemoteParameterConnector::getServerName(const std::string &parameterName)
{
    return _baseName + parameterName;
}

int RemoteParameterConnector::acceptConnection()
{
    return accept(_socketFd, NULL, NULL);
}

int RemoteParameterConnector::getFd() const
{
    return _socketFd;
}

void RemoteParameterConnector::setTimeoutMs(uint32_t timeoutMs)
{
    struct timeval tv;
    tv.tv_sec = timeoutMs / _msecPerSec;
    tv.tv_usec = (timeoutMs % _usecPerMsec) * _usecPerMsec;

    setsockopt(_socketFd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    setsockopt(_socketFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

bool RemoteParameterConnector::send(const void *data, uint32_t size)
{
    AUDIOCOMMS_ASSERT(data != NULL, "NULL data pointer");

    uint32_t offset = 0;
    const uint8_t *pData = static_cast<const uint8_t *>(data);

    while (size) {

        int32_t accessedSize = ::send(_socketFd, &pData[offset], size, MSG_NOSIGNAL);

        if (!accessedSize || accessedSize == -1) {

            return false;
        }
        size -= accessedSize;
        offset += accessedSize;
    }
    return true;
}

bool RemoteParameterConnector::receive(void *data, uint32_t size)
{
    AUDIOCOMMS_ASSERT(data != NULL, "NULL data pointer");

    uint8_t *pData = static_cast<uint8_t *>(data);

    while (size) {

        int32_t accessedSize = ::recv(_socketFd, pData, size, MSG_NOSIGNAL);

        if (!accessedSize || accessedSize == -1) {

            return false;
        }
        size -= accessedSize;
        pData += accessedSize;
    }
    return true;
}

gid_t RemoteParameterConnector::getGid()
{
    struct ucred cr;
    socklen_t len = sizeof(cr);

    if (getsockopt(_socketFd, SOL_SOCKET, SO_PEERCRED, &cr, &len)) {

        ALOGE("could not get socket credentials: %s\n", strerror(errno));
        return -1;
    }

    return cr.gid;
}
