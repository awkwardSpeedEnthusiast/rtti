#pragma once

#include "rtti/property_definition.hpp"

#include <concepts>
#include <meta>
#include <tuple>
#include <type_traits>

/**
 * @brief Extract an existing property definition structure from a class
 *
 * Given the creator of a class added a property defining class as defined in
 * property_definition.hpp,
 * \a find_property_definition() will find it and return the reflection to the struct.
 *
 * Condition for the property struct:
 *    * contains one enum (scoped or non-scoped)
 *    * contains a non-static member field of that enum
 *    * contains a non-static member field of type rtti::property::types
 *
 */
namespace rtti
{
namespace details::property_struct_getting
{

/**
 * @brief returns the reflection of the enum inside the struct
 *
 * returns the reflection of void if the struct doesn't have an enum
 *
 * @tparam i the struct to check
 * @return consteval
 */
template <std::meta::info i>
consteval auto enum_type()
{
  template for (constexpr auto m : std::define_static_array(
                  std::meta::members_of(i, std::meta::access_context::unchecked().via(i))))
  {
    if constexpr (std::meta::is_type(m) && std::meta::is_enum_type(m)) {
      return m;
    }
  }
  return ^^void;
}

/**
 * @brief returns the reflection of the member of type rtti::property::types
 *
 * returns reflection of void if the struct doesn't have such a data member
 *
 * @tparam i reflection of the struct to check
 * @return consteval
 */
template <std::meta::info i>
consteval auto type_member()
{
  template for (constexpr auto m : std::define_static_array(
                  std::meta::members_of(i, std::meta::access_context::unchecked().via(i))))
  {
    if constexpr (std::meta::is_nonstatic_data_member(m)
                  && std::meta::type_of(m) == ^^rtti::property::types) {
      return m;
    }
  }
  return ^^void;
}

/**
 * @brief returns the reflection of the enum member field
 *
 * if field doesn't exist, reflection of void is returned
 *
 * @tparam i reflection of the struct to test
 * @return consteval
 */
template <std::meta::info i>
consteval auto enum_member()
{
  constexpr auto enum_t = enum_type<i>();
  template for (constexpr auto m : std::define_static_array(
                  std::meta::members_of(i, std::meta::access_context::unchecked().via(i))))
  {
    if constexpr (std::meta::is_nonstatic_data_member(m) && std::meta::type_of(m) == enum_t) {
      return m;
    }
  }
  return ^^void;
}
} // namespace details::property_struct_getting

/**
 * @brief extract the reflection of a property struct
 *
 * A property struct is defined by being
 *  * being a type
 *  * private
 *  * containing an enum
 *  * containing a non-static data member of that enum
 *  * containing a non-static data member of type rtti::property::types
 *
 * no checks for duplicates are performed, only the first member (respectively)
 * is taken into account
 *
 * @tparam T the class, which might contain the property definition
 * @return consteval
 */
template <typename T>
consteval auto find_property_definition()
{
  template for (constexpr auto m : std::define_static_array(
                  std::meta::members_of(^^T, std::meta::access_context::unchecked().via(^^T))))
  {
    if constexpr (std::meta::is_type(m) && std::meta::is_private(m)) {
      constexpr auto enum_r = details::property_struct_getting::enum_type<m>();
      if constexpr (!std::meta::is_void_type(enum_r)) {
        constexpr auto type_r = details::property_struct_getting::type_member<m>();
        constexpr auto name_r = details::property_struct_getting::enum_member<m>();
        if constexpr (!std::meta::is_type(type_r) && !std::meta::is_type(name_r)) {
          return m;
        }
      }
    }
  }
  return ^^void;
}

/**
 * @brief property struct definition
 *
 * This struct comes in 2 flavours: this one is for classes, which don't contain
 * property defining structs
 *
 * @tparam T the class of interest
 * @tparam r
 * @tparam valid
 */
template <typename T, std::meta::info r, bool valid>
struct property_t : std::false_type {
};

/**
 * @brief property struct definition
 *
 * This is the second version of the struct for classes, which contain a property
 * defining struct.
 *
 * @tparam T the class of interest
 * @tparam r
 */
template <typename T, std::meta::info r>
struct property_t<T, r, true> : std::true_type {
  using P = [:r:];

  static constexpr auto name_e = details::property_struct_getting::enum_type<r>();
  static constexpr auto name_d = details::property_struct_getting::enum_member<r>();
  static constexpr auto type_d = details::property_struct_getting::type_member<r>();

  using E = [:name_e:];

  static constexpr auto is_valid =
    (!std::meta::is_void_type(name_e)) && std::meta::is_nonstatic_data_member(name_d)
    && std::meta::is_nonstatic_data_member(type_d) && std::meta::is_private(r);
};

/**
 * @brief alias including the search for the property struct
 *
 * @tparam T the class of interest
 */
template <typename T>
using property_struct =
  property_t<T, find_property_definition<T>(), find_property_definition<T>() != ^^void>;
} // namespace rtti
