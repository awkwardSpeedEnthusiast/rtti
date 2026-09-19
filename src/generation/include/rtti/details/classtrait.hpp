#pragma once

#include "rtti/details/methodlist.hpp"

#include <meta>
#include <type_traits>

namespace rtti::details
{
/**
 * @brief basic trait defining properties of a class
 *
 * @tparam T type of the class
 */
template <typename T>
struct class_traits {
  using type_t = std::remove_pointer_t<std::remove_reference_t<std::remove_cv_t<T>>>;

  static constexpr auto const type_name = std::meta::identifier_of(std::meta::dealias(^^T));
  static constexpr auto memberCount =
    std::meta::members_of(^^T, std::meta::access_context::unprivileged()).size();
  using method_tuple = typename method_helper<T, 0, memberCount>::method_tuple;
};
} // namespace rtti::details
