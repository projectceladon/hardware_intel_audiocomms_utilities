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

namespace audio_comms
{
namespace utilities
{

/** Takes an object of type T* and deletes it after calling a method.
 *
 * Function object class.
 */
template <class T, class Ret, Ret(T::*method) ()>
class MethodDelete
{
public:
    /** Call ptr->method then delete ptr */
    void operator()(T *ptr) const
    {
        if (ptr != NULL) {
            (ptr->*method)();
            delete ptr;
        }
    }
};

} // namespace utilities
} // namespace audio_comms
