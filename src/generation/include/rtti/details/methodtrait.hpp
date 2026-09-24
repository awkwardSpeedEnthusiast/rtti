#pragma once

#include "rtti/details/argumentlist.hpp"
#include "rtti/details/metatools.hpp"
#include "rtti/details/tuplebuilder.hpp"

#include <meta>

namespace rtti::details
{
template <std::meta::info Member>
constexpr inline auto to_member_pointer_helper = &[:Member:];

template <std::meta::info method>
struct method_t {
  using result_type_t = [:std::meta::return_type_of(method):];
  using function_ptr_t = [:std::meta::type_of(method):];

  static constexpr auto name = std::meta::identifier_of(method);
  static constexpr auto result_type = ::rtti::detail::type_name<result_type_t>();

  static constexpr auto function_ptr = to_member_pointer_helper<method>;

  using argument_tuple = ::rtti::details::arguments::argument_tuple<method>;
  static constexpr auto argumentNames =
    ::rtti::details::arguments::get_argument_name_array<argument_tuple>();

  static constexpr bool is_const = std::meta::is_const(method);
  static constexpr bool is_virtual = std::meta::is_virtual(method);
  static constexpr bool is_pure_virtual = std::meta::is_pure_virtual(method);
  static constexpr bool is_override = std::meta::is_override(method);
  static constexpr bool is_final = std::meta::is_final(method);
  static constexpr bool is_noexcept = std::meta::is_noexcept(method);
};
} // namespace rtti::details
