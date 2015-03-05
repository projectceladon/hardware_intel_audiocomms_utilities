/* @file
 * Scope tracing class
 *
 * @section License
 *
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

#include "utilities/Log.hpp"
#include "utilities/TypeList.hpp"
#include "utilities/Timestamp.hpp"
#include <pthread.h>
#include <stdint.h>

namespace audio_comms
{
namespace utilities
{

/** Control activation of multiple instance globally.
 * Activate by calling the activate() function.
 * Deactivate by calling the deactivate() function.
 * All new instance of the class will reflect the last activation state.
 * @tparam activateAtStart  boolean that gives the default logging behaviour of the class.
 */
template <bool activateAtStart>
class StaticActivation
{
public:
    StaticActivation() : mActive(mGlobalActive) {}
    bool isActive() { return mActive; }
    static bool isActivated() { return mGlobalActive; }

    /** Activate the logging on the platform for ALL scope objects. */
    static void activate() { mGlobalActive = true; }

    /** Deactivate the logging for ALL scope objects. */
    static void deactivate() { mGlobalActive = false; }

private:
    static volatile bool mGlobalActive;
    const bool mActive;
};

template <bool activateAtStart>
volatile bool StaticActivation<activateAtStart>::mGlobalActive = activateAtStart;

namespace detail
{

template <class A, class B>
struct Inherit : A, B {};

/** Apply to a class all the given Modifier. */
template <class ModifiList>
class ListModifier;

template <class Head, class Tail>
class ListModifier<TypeList<Head, Tail> > : private ListModifier<Tail>
{
public:
    template <class Stream>
    void modify(Stream &stream)
    {
        head.modify(stream);
        ListModifier<Tail>::modify(stream);
    }

private:
    Head head;
};
template <>
class ListModifier<TYPELIST0>
{
public:
    template <class Stream>
    void modify(Stream &) {}
};

} // namespace detail

/** Log class that will modify the logger with the modifier list. */
template <class ModifierList, class Logger>
class ModifiedLogger : public Logger, private detail::ListModifier<ModifierList>
{
public:
    ModifiedLogger() { this->modify(*this); }
};

/** Modify the given stream by adding a prefix. */
template <class Prefix>
struct PrefixModifier : private Prefix
{
    template <class T>
    void modify(T &t)
    {
        t.operator<<(Prefix::get());
    }
};

/** Generate indentation based on the thread instance level. */
template <char Indent>
class IndentPrefix
{
public:
    IndentPrefix() : mThisIndent(mIndentDepth++) {}
    ~IndentPrefix() { mIndentDepth--; }

    std::string get()
    {
        std::string prefix;
        for (size_t i = 0; i < mThisIndent; i++) {
            prefix += Indent;
        }
        return prefix;
    }

private:
    size_t mThisIndent;
    static size_t __thread mIndentDepth;
};

template <char Indent>
size_t __thread IndentPrefix<Indent>::mIndentDepth = 0;

class TimestampModifier
{
public:
    template <class T>
    void modify(T &t)
    {
        t.operator<<(getTimestamp(t));
    }

private:
    template <clockid_t clock>
    uint64_t getTimestamp(const Timestamp<clock> &ts) { return ts.get(); }
};

template <char charSeparator>
struct CharSeparator
{
    char get() { return charSeparator; }
};
typedef PrefixModifier<CharSeparator<' '> > SpacePrefix;

/** Activate a logger based on the given policy. */
template <class ActivationPolicy, class Logger>
class ActivableLogger : public ActivationPolicy
{
public:
    ActivableLogger() : logger(ActivationPolicy::isActive() ? new Logger : NULL) {}
    ~ActivableLogger() { delete logger; }

    template <class T>
    ActivableLogger &operator<<(const T &s)
    {
        if (logger != NULL) {
            logger->operator<<(s);
        }
        return *this;
    }

    Logger *getLogger() { return logger; }

private:
    Logger *logger;
};


/** Activate log with indent and timestamp.
 * @tparam activateAtStart  boolean that gives the default logging behaviour of the class.
 *                          If set to false, the logging may be activated by
 *                          calling the activate() function.
 * @tparam Logger The class that gives the functions used to perform the IO (printing logs).
 *                It may write them in some system log file but also standart outputs or
 *                specific files.
 * @tparam ModifiList A list of modifier to customize the logger format.
 *
 * @example With the default logger and modifiers (timestamp, space, indent),
 *          traces will be formated as such:
 *          <pre>
 *              V/TAG 3312111 Enter firstBlock
 *              V/TAG 3312113  Enter secondEvent
 *              V/TAG 3312114   My bool: 1
 *              V/TAG 3312115  Leaves secondEvent time spent: 2
 *              V/TAG 3312119 Leaves firstBlock time spent: 8
 *          </pre>
 *          If modifiers had been appended to ModifiList eg:
 *          <code>
 *              PrefixModifier<CharSeparator<'|'>,
 *              struct { template <class T> void modify(T &t) { t << std::boolalpha; } }
 *          </code>
 *          Trace format would have changed:
 *          <pre>
 *              V/TAG 3312111 |Enter firstBlock
 *              V/TAG 3312113 | Enter secondEvent
 *              V/TAG 3312114 |  My bool: true
 *              V/TAG 3312115 | Leaves secondEvent time spent: 2
 *              V/TAG 3312119 |Leaves firstBlock time spent: 8
 *          </pre>
 */
template <bool activateAtStart = false,
          class Logger = Log::Verbose,
          class ModifiList = TYPELIST3(TimestampModifier, SpacePrefix,
                                       PrefixModifier<IndentPrefix<' '> > )>
struct  Trace : ActivableLogger<StaticActivation<activateAtStart>,
                                ModifiedLogger<ModifiList,
                                               detail::Inherit<Timestamp<>, Logger> > >
{
#if (__GNUC__ == 4 && __GNUC_MINOR__ == 8 && __GNUC_PATCHLEVEL__ == 0)
    /* Workaround Gcc 4.8 that complains about the inner classes implementation bellow.
     * As we cannot figure out what is the problem (gcc 4.6 is ok, and so is clang), we use this
     * workaround to keep the code compiling. This should obviously be removed when a newer gcc
     * fixes it. */
    typedef Trace<activateAtStart, typeof(*(Logger *) NULL), ModifiList> This;
#else
    typedef Trace This;
#endif

    /** A Scoped class is used to track when execution enters a specified scope.
     *
     * You may create a Scoped object at any place.
     * At object creation a log is written via the logger interface;
     * when the object is destroyed a log is displayed giving the time spent in the scope.
     */
    class Scoped : private This
    {
    public:
        Scoped(const std::string event)
            : mEventName(event)
        {
            Trace() << "Enters " << mEventName;
        }
        ~Scoped()
        {
            // Avoid useless time retrieve by explicitly testing activation state.
            if (this->isActive()) {
                uint64_t startTime = this->getLogger()->Timestamp<>::get();
                uint64_t stopTime = Timestamp<>().get();
                *this << "Leaves " << mEventName
                      << " time spent: " << stopTime - startTime << "us";
            }
        }
        template <class T>
        void trace(const T &val)
        {
            Trace() << "trace " << mEventName << " value=" << val;
        }

    private:
        const std::string mEventName;

    public:

        /** Scoped event that can be constructed from an Event type.
         *
         * @tparam EventTrait must contain the getEventName that converts
         *        the constructor event param to a string.
         */
        template <class EventTrait>
        struct Event : public This::Scoped
        {
            Event(const typename EventTrait::Event &event)
                : Scoped(EventTrait::getEventName(event)) {}
        };
    };

};

} /* namespace utilities */
} /* namespace audio_comms */
