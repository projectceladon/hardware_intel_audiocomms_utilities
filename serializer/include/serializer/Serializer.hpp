/**
 * @section License
 *
 * Copyright 2013-2014 Intel Corporation
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

namespace audio_comms
{
namespace utilities
{
namespace serializer
{

/**
 * This template class has to be specialized to describe the way the class
 * Class has to be serialized
 */
template <class Class>
class ClassSerializationTrait;

template <class Result>
class Serializer
{
public:
    template <class Class>
    static Result serialize(const Class &c, std::string &serializedCmd);

    template <class Class>
    static Result deserialize(const std::string &str, Class &c);
};

} // namespace serializer
} // namespace utilities
} // namespace audio_comms
