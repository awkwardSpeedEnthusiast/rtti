#pragma once

#include "rtti/details/argumentlist.hpp"
#include "rtti/details/typetools.hpp"

#include <meta>

namespace rtti::details
{
template <std::meta::info Member>
constexpr inline auto to_member_pointer_helper = &[:Member:];

template <typename T, size_t I>
struct method_traits {
  static constexpr auto meth_i =
    std::meta::members_of(^^T, std::meta::access_context::unprivileged())[I];

  using result_type_t = [:std::meta::return_type_of(meth_i):];
  using function_ptr_t = [:std::meta::type_of(meth_i):];

  static constexpr auto name = std::meta::identifier_of(meth_i);
  static constexpr auto result_type =
    ::rtti::details::getTypeName<std::meta::return_type_of(meth_i)>();

  static constexpr auto function_ptr = to_member_pointer_helper<meth_i>;

  static constexpr auto argumentCount = std::meta::parameters_of(meth_i).size();
  using argument_tuple =
    typename ::rtti::details::arguments::args_helper<T, I, 0, argumentCount>::arg_tuple;
  static constexpr auto argumentNames =
    ::rtti::details::arguments::get_argument_name_array<argument_tuple>();

  static constexpr bool is_const = std::meta::is_const(meth_i);
  static constexpr bool is_virtual = std::meta::is_virtual(meth_i);
  static constexpr bool is_pure_virtual = std::meta::is_pure_virtual(meth_i);
  static constexpr bool is_override = std::meta::is_override(meth_i);
  static constexpr bool is_final = std::meta::is_final(meth_i);
  static constexpr bool is_noexcept = std::meta::is_noexcept(meth_i);
};
} // namespace rtti::details
