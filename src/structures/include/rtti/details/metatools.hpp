#pragma once

#if defined(__cpp_impl_reflection) && defined(__cpp_impl_reflection)
#  define MY_USE_REFLECTION
#  include <meta>

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

/**
 * @brief retrieve the identifier/name of a reflectin
 *
 * @tparam i the reflection in question
 * @return consteval
 */
template <std::meta::info i>
consteval auto item_name()
{
  if constexpr (std::meta::has_identifier(i)) {
    return std::meta::identifier_of(i);
  } else if constexpr (std::meta::is_function_parameter(i)) {
    return "<arg>";
  } else {
    std::meta::display_string_of(i);
  }
}

} // namespace rtti::detail
#endif
