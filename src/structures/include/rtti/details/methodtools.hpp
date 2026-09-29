#pragma once

#include "rtti/details/metatools.hpp"

#include <any>
#include <functional>
#include <meta>
#include <ranges>
#include <string_view>
#include <vector>

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
                || std::meta::is_variable(mem) || std::meta::is_static_member(mem)
                || std::meta::is_operator_function(mem)) {
    return false;
  } else if constexpr (std::meta::is_function(mem)) {
    return true;
  } else {
    return false;
  }
}

template <auto parameter>
using TypeOfVariable = typename[:std::meta::type_of(parameter):];

template <auto Member, typename ClassType = void>
std::any universal_invoker(std::vector<std::any> args)
{
  constexpr auto params = std::define_static_array(std::meta::parameters_of(Member));

  return []<std::size_t... Is>(std::vector<std::any> args, std::index_sequence<Is...>) {
    constexpr auto params = std::define_static_array(std::meta::parameters_of(Member));

    if constexpr (!std::meta::is_void_type(^^ClassType) && !std::meta::is_static_member(Member)) {
      auto* obj = std::any_cast<ClassType*>(args[0]);
      if constexpr (std::meta::is_void_type(std::meta::return_type_of(Member))) {
        std::invoke(&[:Member:], obj, std::any_cast<TypeOfVariable<params[Is]>>(args[Is + 1])...);
        return std::any {};
      } else {
        return std::any(std::invoke(
          &[:Member:], obj, std::any_cast<TypeOfVariable<params[Is]>>(args[Is + 1])...));
      }
    } else {
      if constexpr (std::meta::is_void_type(std::meta::return_type_of(Member))) {
        std::invoke(&[:Member:], std::any_cast<TypeOfVariable<params[Is]>>(args[Is])...);
        return std::any {};
      } else {
        return std::any(
          std::invoke(&[:Member:], std::any_cast<TypeOfVariable<params[Is]>>(args[Is])...));
      }
    }
  }(args, std::make_index_sequence<params.size()> {});
}

using ErasedInvoker = std::any (*)(std::vector<std::any> args);

template <auto M>
std::vector<std::string_view> ArgumentListCreator_i()
{
  std::array<std::string_view, std::define_static_array(std::meta::parameters_of(M)).size()>
    result {};
  std::size_t idx [[maybe_unused]] = 0;

  template for (constexpr auto a : std::define_static_array(std::meta::parameters_of(M)))
  {
    result[idx++] = rtti::detail::item_name<a>();
  }

  return result | std::ranges::to<std::vector>();
}

using ArgumentListCreator = std::vector<std::string_view> (*)();

struct f_info {
  std::string_view name;
  std::string_view returnType;
  ErasedInvoker invoke;
  bool is_static;
  bool is_const;
  bool is_override;
  bool is_virtual;
  bool is_pure_virtual;
  bool is_final;
  bool is_member;
  bool is_lambda;
  ArgumentListCreator alc;
};

template <std::meta::info method, typename T>
consteval f_info createFromType()
{
  auto createname = []<auto info>() {
    if constexpr (std::meta::has_identifier(info)) {
      return std::meta::identifier_of(info);
    } else {
      return std::meta::display_string_of(info);
    }
  };
  return f_info {
    .name = createname.template operator()<method>(),
    .returnType = rtti::detail::type_name<typename[:std::meta::return_type_of(method):]>(),
    .invoke = &universal_invoker<method, T>,
    .is_static = std::meta::is_static_member(method),
    .is_const = std::meta::is_const(method),
    .is_override = std::meta::is_override(method),
    .is_virtual = std::meta::is_virtual(method),
    .is_pure_virtual = std::meta::is_pure_virtual(method),
    .is_final = std::meta::is_final(method),
    .is_member = !std::meta::is_void_type(^^T),
    .is_lambda = false,
    .alc = &ArgumentListCreator_i<method>,
  };
}
} // namespace rtti::details
