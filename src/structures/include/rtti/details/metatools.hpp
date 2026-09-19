#pragma once

#include <meta>

namespace rtti::detail
{
/**
 * @brief return the name of the type
 *
 * @tparam T the type to query the name from
 * @return std::string_view
 */
template <typename T>
consteval auto type_name()
{
  constexpr auto type = std::meta::dealias(^^T);
  static_assert(std::meta::is_type(type), "requires \"type\" to be a type");
  if constexpr (std::same_as<T, void>) {
    return "void";
  } else if constexpr (std::meta::has_identifier(type)) {
    return std::meta::identifier_of(type);
  } else {
    return std::meta::display_string_of(type);
  }
}
} // namespace rtti::detail
