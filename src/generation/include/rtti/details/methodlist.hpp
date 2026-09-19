#pragma once

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
 * @brief convert the member reflection to a tuple of method traits
 *
 * this structure and the two following ones (template specializations) do the heavy lifting of
 * extracting information about possible method members out of a class. It is called recursively
 * (the index \a I incrementing from 0 to \a N) adding the method_trait item to the \a Args...
 * parameter pack if a method of interest until the number of members \a N is reached.
 * There the parameter pack is converted to a tuple type.
 *
 * Well, actually no function is called, but types are constructed. Doesn't matter, works anyway.
 *
 * @tparam T
 * @tparam I
 * @tparam N
 * @tparam Args
 */
template <typename T, size_t I, size_t N, typename... Args>
struct method_helper {
  static constexpr auto meth_i =
    std::meta::members_of(^^T, std::meta::access_context::unprivileged())[I];
  using method_tuple = typename std::conditional_t<
    isFunction<meth_i>(),
    typename method_helper<T, I + 1, N, rtti::details::method_traits<T, I>, Args...>::method_tuple,
    typename method_helper<T, I + 1, N, Args...>::method_tuple>;
};
/**
 * @brief no method recursion break case
 */
template <typename T, size_t N>
struct method_helper<T, N, N> {
  using method_tuple = std::tuple<>;
};
/**
 * @brief recursion break case
 */
template <typename T, size_t N, typename... Args>
struct method_helper<T, N, N, Args...> {
  using method_tuple = std::tuple<Args...>;
};

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
