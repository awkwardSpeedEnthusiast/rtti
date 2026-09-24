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
  using method_tuple = typename methodList_builder<^^T>::tuple_type;
};
} // namespace rtti::details
