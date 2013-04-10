#ifndef AUDIOCOMMS_ASSERT_H
#define AUDIOCOMMS_ASSERT_H

#ifdef __cplusplus

/**
  *  This AUDIOCOMMS_COMPILE_TIME_ASSERT MACRO will fail to compile if the condition is false.
  *  Currently it only works in functions
  *  Example:
  *      Put "AUDIOCOMMS_COMPILE_TIME_ASSERT(1==0)" your C++ source.
  *  On compilation you will get the following error:
  *      audioCommsAssert.h:25: error: 'audioComms::compileTimeAssertFailure<false>::compileTimeAssertFailure()' is private
  *      YOUR_SOURCE_FILE.cpp:YOUR_ASSERT_LINE: error: within this context
  */
#define AUDIOCOMMS_COMPILE_TIME_ASSERT(c) audiocomms::compileTimeAssertFailure<c>()

namespace audiocomms
{
    template <bool condition>
    class compileTimeAssertFailure {
        private:
        compileTimeAssertFailure()
    };

    // If true do nothing
    template <>
    class compileTimeAssertFailure<true> {};

    // If false instaciate with private constructor
    template <>
    class compileTimeAssertFailure<false>
    {
        inline compileTimeAssertFailure() {};
    };
}
#else // __cplusplus

/**
  *  This AUDIOCOMMS_COMPILE_TIME_ASSERT MACRO will fail to compile if the condition is false.
  *  Currently it only works in functions
  *  Example:
  *      Put "AUDIOCOMMS_COMPILE_TIME_ASSERT(1==0)" your C source.
  *  On compilation you will get the following error:
  *      YOUR_SOURCE_FILE.c:YOUR_ASSERT_LINE: error: duplicate case value
  *      YOUR_SOURCE_FILE.c:YOUR_ASSERT_LINE: error: previously used here
  */
#define AUDIOCOMMS_COMPILE_TIME_ASSERT(expr)   \
    switch(0) { \
        case 0: \
        case expr: \
        ; \
    }

#endif // __cplusplus


#endif // AUDIOCOMMS_ASSERT_H

