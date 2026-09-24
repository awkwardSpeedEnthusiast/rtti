#pragma once

#include "rtti/details/tuplebuilder.hpp"
#include "rtti/method_extraction.hpp"

#include <meta>
#include <tuple>
#include <type_traits>

namespace rtti::details
{
/**
 * @brief queries whether the member is a function type of interest
 *
 * @tparam mem the member reflection
 * @return bool
 */
template <std::meta::info mem>
consteval bool isFunction()
{
  if constexpr (std::meta::is_move_constructor(mem) || std::meta::is_default_constructor(mem)
                || std::meta::is_copy_constructor(mem) || std::meta::is_constructor(mem)
                || std::meta::is_destructor(mem) || std::meta::is_copy_assignment(mem)
                || std::meta::is_move_assignment(mem) || std::meta::is_assignment(mem)
                || std::meta::is_variable(mem) || std::meta::is_static_member(mem)) {
    return false;
  } else if constexpr (std::meta::is_function(mem)) {
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Predicate to be used to accept methods
 *
 * @tparam m the method reflection
 */
template <std::meta::info m>
struct isFunction_t {
  static constexpr auto value = rtti::details::isFunction<m>();
};

/**
 * @brief extract the list of members from a class reflection
 *
 * @tparam c the class reflection
 */
template <std::meta::info c>
struct members {
  static constexpr auto list =
    std::define_static_array(std::meta::members_of(c, std::meta::access_context::unprivileged()));
};

/**
 * @brief Alias for extracting information about a list of methods inside a class
 *
 * Using the tuple builder structure, we can fix some of the strategies for the
 * use with method lists:
 *   * \a members shall be extracted
 *   * only function members shall be taken (no operators, constructors, etc.)
 *   * the trait type is \a method_t
 *
 * This leaves the class reflection as only template parameter
 *
 * @tparam clazz
 */
template <std::meta::info clazz>
using methodList_builder =
  rtti::tuple_builder_t<clazz, rtti::details::members, rtti::details::isFunction_t, method_t>;

/**
 * @brief Get the array from tuple object
 *
 * This little helper function converts a tuple type of method_traits to an array of \a
 * rtti::MetaMethod objects.
 *
 * @tparam tuple_t the tuple type to convert
 * @return std::array<rtti::MetaMethod>
 */
template <typename tuple_t>
constexpr auto get_array_from_tuple()
{
  if constexpr (std::tuple_size_v<tuple_t> == 0) {
    return std::array<rtti::MetaMethod, 0> {};
  } else {
    constexpr auto convert = []<typename T>(T&&) { return createMethod<T>(); };
    constexpr auto get_array = [convert]<typename... Args>(auto&&... x) {
      return std::array { convert(std::forward<decltype(x)>(x))... };
    };
    return std::apply(get_array, tuple_t {});
  }
}
} // namespace rtti::details
