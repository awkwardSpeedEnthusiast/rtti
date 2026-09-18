#pragma once

#include <meta>

namespace rtti::details
{
/**
 * @brief Get the Type Name object
 *
 * returns a string representation of a type reflection.
 *
 * A method is necessary, because a void type doesn't have an identifier.
 * Some stl types also don't seem to have an identifier (could be template?).
 *
 * @tparam var the type reflection
 * @return std::string_view
 */
template <std::meta::info var>
consteval auto getTypeName()
{
  if constexpr (std::meta::is_void_type(var)) {
    return "void";
  } else if constexpr (std::meta::has_identifier(var)) {
    return std::meta::identifier_of(var);
  } else {
    return "<unknown>";
  }
}
} // namespace rtti::details
