/*
 * Copyright 2013-2016 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <AudioNonCopyable.hpp>
#include <AudioCommsAssert.hpp>
#include <convert/convert.hpp>
#include <string>
#include <cstdlib>


namespace audio_comms
{

namespace utilities
{

/**
 * SystemProperty Presents the default API that a system property should have.
 */
class SystemProperty
{
public:
    /**
     * get the value of a property
     *
     * @param[in] name   the property's name
     *
     * @return the property's value.
     */
    virtual std::string get(const std::string &name) const = 0;

    /**
     * set a property
     *
     * @param[in] name   the property's name
     * @param[in] val    the property's value to be set.
     *
     * @return true in case of success, false otherwise.
     */
    virtual bool set(const std::string &name, const std::string &val) = 0;

    virtual ~SystemProperty() {}
};

/** property_details namespace is here to hide implementation details to end user. It
 * is NOT intended to be used outside. */
#ifdef __ANDROID__
namespace property_details
{
class AndroidProperty;
}
/** On Android, we use the AndroidProperty as the default one */
typedef property_details::AndroidProperty DefaultSystemProperty;

#else  // __ANDROID__
namespace property_details
{
class LinuxProperty;
}
/** On Linux, we use the LinuxProperty as the default one */
typedef property_details::LinuxProperty DefaultSystemProperty;
#endif // __ANDROID__

namespace property_details
{
template <typename T>
class Converter;
}

/**
 * Property class to wrap Android's property facility
 *
 * tparam T                 Type of the property's value.
 * tparam SystemProperty    Type of the System Property that will be used for
 *                          the instance.  This allows to change the low level
 *                          facility used to handle properties.
 */
template <class T, class SystemProperty = DefaultSystemProperty>
class Property : public NonCopyable
{
public:
    /**
     * Property's constructor with default value
     *
     * @param[in] name          Property's name
     * @param[in] defaultValue  default value of the property. If missing,
     *                          default constructor of type is called.
     */
    explicit Property(const std::string &name, const T &defaultValue = T())
        : _sysProp(new SystemProperty()), _name(name), _default(defaultValue)
    {
        /* As we cannot use exceptions, having an error here is just about
         * aborting execution in order to make sure that there is not silent fail. */
        AUDIOCOMMS_ASSERT(_sysProp != NULL, "Cannot allocate memory for property.");
        AUDIOCOMMS_ASSERT(_name.length() + sizeof('\0') <= SystemProperty::nameMaxSize,
                          "property name '" << _name << "' is too long (max is "
                                            << SystemProperty::nameMaxSize << ").");

        std::string valAsStr;
        AUDIOCOMMS_ASSERT(property_details::Converter<T>::valueToString(_default, valAsStr)
                          && valAsStr.length() + sizeof('\0') <= SystemProperty::valueMaxSize,
                          "default value " << _default << " is too long (max is "
                                           << SystemProperty::valueMaxSize << ").");
    }

    ~Property()
    {
        delete _sysProp;
    }

    /**
     * Set the property's value
     *
     * @param[in] val  the value to set
     *
     * @return true upon success, false otherwise
     */
    inline bool setValue(const T &val)
    {
        std::string valAsStr;

        if (!property_details::Converter<T>::valueToString(val, valAsStr)
            || valAsStr.length() + sizeof('\0') > SystemProperty::valueMaxSize) {
            return false;
        }

        return _sysProp->set(_name, valAsStr);
    }

    /**
     * Get the property's value
     *
     * @return the property's value. An empty string may mean that the property
     *         is either inexistent or empty.
     */
    inline T getValue() const
    {
        std::string prop(_sysProp->get(_name));
        T result = _default;
        if (prop.empty() || !property_details::Converter<T>::stringToValue(prop, result)) {
            return _default;
        }
        return result;
    }

private:
    /** Here I chose to have a pointer on the system property rather than
     * inherit from it for 2 reasons:
     * - the Property we build here does not follow the same semantic than the
     *   original Android's property.
     * - check that the template parameter passed inherits from SystemProperty
     * is easier to do. */
    SystemProperty *_sysProp; /*< handle on the system property methods */

    std::string _name;        /*< the property's name */
    T _default;               /*< the property's default value */
};

/**
 * Implementation details of property class, not intended to be used by any
 * third party component */
namespace property_details
{

/**
 * Helper class to select is conversion is needed or not.
 */
template <typename T>
class Converter
{
public:
    /**
     * Convert a string to a numerical value using \see { convertTo }
     *
     * @tparam     T        type of the result
     * @param[in]  str      string to convert
     * @param[out] result   conversion result
     *
     * @return true on success, false otherwise
     */
    static bool stringToValue(const std::string &str, T &result)
    {
        return audio_comms::utilities::convertTo<std::string, T>(str, result);
    }

    /**
     * Convert a numerical value to its string representation (base 10)
     *
     * @tparam     T        type of the result
     * @param[in]  val      value to convert
     * @param[out] result   result string
     *
     * @return true on success, false otherwise
     */
    static bool valueToString(const T &val, std::string &result)
    {
        return audio_comms::utilities::convertTo<T, std::string>(val, result);
    }
};

} // namespace property_details

} // namespace utilities

} // namespace audio_comms

#ifdef __ANDROID__

#include <cutils/properties.h>
namespace audio_comms
{

namespace utilities
{

namespace property_details
{

/**
 * Android property's access class.
 * This class standardize wrap the access to AOSP property's functions.
 */
class AndroidProperty : public SystemProperty
{
/** Constructor is made private to prevent the use of this class by any
 * other class than property */
template <class T, class U>
friend class audio_comms::utilities::Property;
AndroidProperty()
{
    /** Next assertion are here for consistency checks, if they fail, this
     * means that android's properties sizes changed, which means the
     * modification MUST be done on the Linux size (bellow) */
    AUDIOCOMMS_COMPILE_TIME_ASSERT(nameMaxSize == 32);
    AUDIOCOMMS_COMPILE_TIME_ASSERT(valueMaxSize == 92);
}

public:
    static const size_t nameMaxSize = PROPERTY_KEY_MAX;
    static const size_t valueMaxSize = PROPERTY_VALUE_MAX;

    std::string get(const std::string &name) const
    {
        char propValue[valueMaxSize];

        property_get(name.c_str(), propValue, "");

        return propValue;
    }

    bool set(const std::string &name, const std::string &val)
    {
        return property_set(name.c_str(), val.c_str()) >= 0;
    }
};


} // namespace property_details

} // namespace utilities

} // namespace audio_comms

#else // __ANDROID__

#include <map>
#include <fstream>

namespace audio_comms
{

namespace utilities
{

namespace property_details
{

/**
 * The Linux implementation for Android's property.
 * This reads and writes properties from/to a file. The file is read when
 * creating the object, which means that any (external) modification of the
 * file will have not effect until a new property object is created.
 */
class LinuxProperty : public SystemProperty
{
/** Constructor is made private to prevent the use of this class by any
 * other class than property */
LinuxProperty()
{
    _propFile.open(_fileName.c_str(), std::fstream::in);
    readFromFile();
    _propFile.close();
}

template <class T, class U>
friend class audio_comms::utilities::Property;

public:
    /**
     * Next two values could be set to arbitrary values, but for
     * 'compatibility' purpose for user, I set the Android's values.
     */
    static const size_t nameMaxSize = 32;
    static const size_t valueMaxSize = 92;

    ~LinuxProperty()
    {
        _propFile.open(_fileName.c_str(), std::fstream::out);
        writeToFile();
        _propFile.close();
    }

    /**
     * Set a property in the Linux property operational layer.
     *
     * @param[in] name   Property's name
     * @param[in] val    Property's value
     *
     * @return true on success, false otherwise
     */
    virtual bool set(const std::string &name, const std::string &val)
    {
        _properties[name] = val;
        return true;
    }

    /**
     * Get a property from the Linux' property operational layer.
     *
     * @param[in] name   Property's name
     *
     * @return the property's value or an empty string in case of property does
     * not exists or it is empty.
     */
    virtual std::string get(const std::string &name) const
    {
        std::map<std::string, std::string>::const_iterator it;
        it = _properties.find(name);
        return it != _properties.end() ? it->second : "";
    }

    /**
     * Set the name of the property file.
     * This function is just for unit test use purpose.
     *
     * @param[in] name   the new name
     */
    static void setPropFileName(const std::string &name)
    {
        _fileName = name;
    }

private:
    static std::string _fileName;

    std::fstream _propFile;
    std::map<std::string, std::string> _properties;

    /**
     * Write all properties of _properties' map into the file named _fileName
     */
    void writeToFile()
    {
        _propFile.seekp(0, std::ios::beg);
        std::map<std::string, std::string>::const_iterator it;
        for (it = _properties.begin(); it != _properties.end(); ++it) {
            _propFile << it->first << "=" << it->second << std::endl;
        }
    }

    /*
     * Read all properties from the file named _fileName.
     * Entries must be in the format property_name=property_value
     */
    void readFromFile()
    {
        std::string line;

        while (std::getline(_propFile, line)) {
            std::string name, value;
            std::string::size_type equal = line.find('=', 0);

            /* Extract the property name */
            name = line.substr(0, equal);

            /* read from the first character just after '=' */
            value = line.substr(equal + 1, line.size());

            _properties[name] = value;
        }
    }
};

} // namespace property_details

} // namespace utilities

} // namespace audio_comms
#endif // __ANDROID__
