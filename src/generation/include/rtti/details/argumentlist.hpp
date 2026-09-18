#pragma once

#include "rtti/details/typetools.hpp"

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
 * I'm not particularly sure whether the <Type, M_index, Arg_index> is the optimal way to identify.
 *
 * Next, try with <std::meta::info> as NTTP.
 *
 * @tparam T The class the method belongs to
 * @tparam methodI the index of the method in the class
 * @tparam argI the index of the argument
 */
template <typename T, size_t methodI, size_t argI>
struct argument_trait {
  static constexpr auto meth_i =
    std::meta::members_of(^^T, std::meta::access_context::unprivileged())[methodI];
  static constexpr auto arg_i = std::meta::parameters_of(meth_i)[argI];

  using type_t = [:std::meta::type_of(arg_i):];

  static constexpr auto name = std::meta::identifier_of(arg_i);
  static constexpr auto type_name = getTypeName<std::meta::type_of(arg_i)>();

  static constexpr bool is_const = std::meta::is_const(arg_i);
  static constexpr bool is_pointer = std::meta::is_pointer_type(arg_i);
  static constexpr bool is_reference = std::meta::is_reference_type(arg_i);
};

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

/**
 * @brief generate argument-trait tuple
 *
 * These 3 helper structs generate a tuple of argument-traits for an argument-list of a member
 * function of a class. This struct is generated recursively collecting the traits in the \a Args
 * parameter pack, incrementing the current argument index \a J. When the index reaches the total
 * size, the tuple is generated (last of the three specializations).
 *
 * There is one specialization for the case, no arguments are present
 *
 * @tparam T the class to consider (const)
 * @tparam I the index of the method (const)
 * @tparam J the current argument index
 * @tparam N the argument count of the method
 * @tparam Args collected argument traits
 */
template <typename T, size_t I, size_t J, size_t N, typename... Args>
struct args_helper {
  using arg_tuple =
    typename args_helper<T, I, J + 1, N, argument_trait<T, I, J>, Args...>::arg_tuple;
};
// case N == 0
template <typename T, size_t I, size_t N>
struct args_helper<T, I, N, N> {
  using arg_tuple = std::tuple<>;
};
// recursion end
template <typename T, size_t I, size_t N, typename... Args>
struct args_helper<T, I, N, N, Args...> {
  using arg_tuple = std::tuple<Args...>;
};

} // namespace rtti::details::arguments
