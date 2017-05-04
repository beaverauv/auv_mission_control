// This file is part of Better Enums, released under the BSD 2-clause license.
// See doc/LICENSE for details, or visit http://github.com/aantron/better-enums.

#pragma once

#ifndef BETTER_ENUMS_ENUM_H
#define BETTER_ENUMS_ENUM_H

#include <string>
#include <unordered_map>

//----------------------------------------------------------------------------//
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define ROBOT_ENUM(type)                                                       \
  template <>                                                                  \
  std::unordered_map<std::string, type> Enum<type>::mMap =                     \
      std::unordered_map<std::string, type>();                                 \
  template <> Enum<type>::Enum(void)

////////////////////////////////////////////////////////////////////////////////

#define _ROBOT_ENUM_MAP_1(value) mMap[#value] = value;
#define _ROBOT_ENUM_MAP_2(value, key) mMap[key] = value;

#define _ROBOT_ENUM_SELECT(_1, _2, NAME, ...) NAME
#define _ROBOT_ENUM_EXPAND(expression) expression

#define ROBOT_ENUM_MAP(...)                                                    \
  _ROBOT_ENUM_EXPAND(_ROBOT_ENUM_SELECT(__VA_ARGS__, _ROBOT_ENUM_MAP_2,        \
                                        _ROBOT_ENUM_MAP_1)(__VA_ARGS__))

//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

template <typename Type> class Enum {
public:
  //----------------------------------------------------------------------------//
  // Types //
  //----------------------------------------------------------------------------//

  ////////////////////////////////////////////////////////////////////////////////

  typedef std::unordered_map<std::string, Type> ValueMap;

private:
  //----------------------------------------------------------------------------//
  // Constructors //
  //----------------------------------------------------------------------------//

  ////////////////////////////////////////////////////////////////////////////////

  Enum(void);

public:
  //----------------------------------------------------------------------------//
  // Functions //
  //----------------------------------------------------------------------------//

  ////////////////////////////////////////////////////////////////////////////////

  static uintptr_t Size(void) {
    // Initialize the static parser
    if (mMap.empty())
      Enum<Type>();

    // Return map size
    return mMap.size();
  }

  ////////////////////////////////////////////////////////////////////////////////

  static const ValueMap &GetMap(void) {
    // Initialize the static parser
    if (mMap.empty())
      Enum<Type>();

    return mMap;
  }

  ////////////////////////////////////////////////////////////////////////////////

  static Type Parse(const std::string &key, Type defaultValue = (Type)-1) {
    // Initialize the static parser
    if (mMap.empty())
      Enum<Type>();

    // Search for a matching key
    return mMap.find(key) != mMap.end() ? mMap.at(key) : defaultValue;
  }

  ////////////////////////////////////////////////////////////////////////////////

  static std::string Parse(Type value, const char *defaultValue = "") {
    // Initialize the static parser
    if (mMap.empty())
      Enum<Type>();

    // Search for a matching value
    for (auto i = mMap.begin(); i != mMap.end(); ++i) {
      if (i->second == value)
        return i->first;
    }

    // Nothing was found
    return defaultValue;
  }

private:
  //----------------------------------------------------------------------------//
  // Fields //
  //----------------------------------------------------------------------------//

  static ValueMap mMap;
};

#endif // #ifndef BETTER_ENUMS_ENUM_H
