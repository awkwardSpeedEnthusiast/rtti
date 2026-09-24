#pragma once

#include "rtti/details/metatools.hpp"
#include "rtti/details/tuplebuilder.hpp"

#include <meta>
#include <tuple>
#include <type_traits>

/**
 * @brief Extraction of argument list of methods
 *
 * The argument list of methods is not that straight forward, taking into account, that we have to
 * do it all at compile time.
 * Easiest it seems is to put all into static members of a struct, kind of a trait, and use this
 * type in a tuple to reflect the argument list.
 * With some hack, we then can convert this tuple to some array and use that in MetaMethod creation.
 */
namespace rtti::details::arguments
{
/**
 * @brief properties of one method argument
 *
 * @tparam arg The reflection of the function argument
 */
template <std::meta::info arg>
struct argument_t {
  using type_t = [:std::meta::type_of(arg):];

  static constexpr auto name = std::meta::identifier_of(arg);
  static constexpr auto type_name = rtti::detail::type_name<type_t>();

  static constexpr bool is_const = std::meta::is_const(arg);
  static constexpr bool is_pointer = std::meta::is_pointer_type(arg);
  static constexpr bool is_reference = std::meta::is_reference_type(arg);
};

/**
 * @brief extract the parameter list from a function reflection
 *
 * @tparam m the function reflection
 */
template <std::meta::info m>
struct parameters {
  static constexpr auto list = std::define_static_array(std::meta::parameters_of(m));
};

/**
 * @brief Predicate to be used to accept arguments
 *
 * All arguments shall be accepted, so always true.
 *
 * @tparam a the function argument reflection
 */
template <std::meta::info a>
struct true_pred : std::true_type {
};

/**
 * @brief Alias for extracting information about an argument list of a function
 *
 * Using the tuple builder structure, we can fix some of the strategies for the
 * use with argument lists:
 *   * \a parameters shall be extracted
 *   * all arguments shall be taken
 *   * the trait type is \a argument_t
 *
 * This leaves the function reflection as only template parameter
 *
 * @tparam method the function reflection
 */
template <std::meta::info method>
using argument_tuple = rtti::tuple_builder_t<method, parameters, true_pred, argument_t>::tuple_type;

/**
 * @brief Convert the argument-list tuple to an array of argument names
 *
 * @tparam tuple_t argument-list trait tuple
 * @return std::array<std::string_view, N>
 */
template <typename tuple_t>
constexpr auto get_argument_name_array()
{
  if constexpr (std::tuple_size_v<tuple_t> == 0) {
    return std::array<std::string_view, 0> {};
  } else {
    constexpr auto convert = []<typename T>(T&&) { return T::name; };
    constexpr auto get_array = [convert]<typename... Args>(auto&&... x) {
      return std::array { convert(std::forward<decltype(x)>(x))... };
    };
    return std::apply(get_array, tuple_t {});
  }
}

} // namespace rtti::details::arguments
