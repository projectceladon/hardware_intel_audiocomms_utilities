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

#define LOG_TAG "RemoteParameterServer"

#include "RemoteParameterServer.hpp"
#include "RemoteParameter.hpp"
#include "RemoteParameterImpl.hpp"
#include <AudioCommsAssert.hpp>
#include "EventThread.h"
#include <utils/Log.h>

using std::string;

RemoteParameterServer::RemoteParameterServer()
    : _eventThread(new CEventThread(this)),
      _fdClientId(0),
      _started(false)
{
}

RemoteParameterServer::~RemoteParameterServer()
{
    stop();

    // Destroy remote parameters
    RemoteParameterImplMapIterator it;

    for (it = _remoteParameterImplMap.begin(); it != _remoteParameterImplMap.end(); ++it) {

        delete it->second;
        it->second = NULL;
    }

    // Event Thread
    delete _eventThread;
    _eventThread = NULL;
}

bool RemoteParameterServer::addRemoteParameter(RemoteParameterBase *remoteParameter, string &error)
{
    AUDIOCOMMS_ASSERT(remoteParameter != NULL, "invalid remote parameter");
    if (_started || (_started && _eventThread->inThreadContext())) {

        error = "Parameter added in not permitted context";
        return false;
    }

    // Check if the parameter name has already been added (opening twice the file descriptor would
    // block any client to connect to the file descriptor of the socket
    RemoteParameterImplMapConstIterator it;
    for (it = _remoteParameterImplMap.begin(); it != _remoteParameterImplMap.end(); ++it) {

        RemoteParameterImpl *implementor = it->second;
        if (remoteParameter->getName() == implementor->getName()) {

            error = "Parameter Name already added";
            return false;
        }
    }

    // Create new Remote Parameter Implementor
    RemoteParameterImpl *implementor = new RemoteParameterImpl(remoteParameter,
                                                               remoteParameter->getName(),
                                                               remoteParameter->getSize());
    // Check not already inserted
    if (_remoteParameterImplMap.find(implementor->getPollFd()) != _remoteParameterImplMap.end()) {

        error = "Poll Fd added twice";
        delete implementor;
        return false;
    }

    // Record parameter
    _remoteParameterImplMap[implementor->getPollFd()] = implementor;

    // Listen to new server requests
    _eventThread->addOpenedFd(_fdClientId++, implementor->getPollFd(), true);

    return true;
}

bool RemoteParameterServer::start()
{
    bool status = _started ? false : _eventThread->start();
    _started = true;
    return status;
}

void RemoteParameterServer::stop()
{
    _eventThread->stop();
    _started = false;
}

bool RemoteParameterServer::onEvent(int fd)
{
    ALOGD("%s", __FUNCTION__);

    // Find appropriate server
    RemoteParameterImplMapConstIterator it = _remoteParameterImplMap.find(fd);

    if (it == _remoteParameterImplMap.end()) {

        ALOGE("%s: remote parameter not found!", __FUNCTION__);
    } else {

        // Process request
        RemoteParameterImpl *remoteParameterImpl = it->second;

        remoteParameterImpl->handleNewConnection();
    }

    return false;
}

bool RemoteParameterServer::onError(int fd)
{
    // Nothing to do
    return false;
}

bool RemoteParameterServer::onHangup(int fd)
{
    // Nothing to do
    return false;
}

void RemoteParameterServer::onAlarm()
{
    ALOGE("%s: server timeout!", __FUNCTION__);
}

void RemoteParameterServer::onPollError()
{
    ALOGE("%s: server poll error!", __FUNCTION__);
}

bool RemoteParameterServer::onProcess(uint16_t eventId)
{
    // Nothing to do
    return false;
}
