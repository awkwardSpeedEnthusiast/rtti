#pragma once

#include "rtti/details/argumentlist.hpp"
#include "rtti/details/methodtrait.hpp"
#include "rtti/details/typetools.hpp"

#include "rtti/metamethod.hpp"

#include <array>
#include <iterator>
#include <meta>
#include <ranges>
#include <string_view>

/**
 * @brief Create a Method object
 *
 * Create a RunTime Type Introspection object for a class member function
 *
 * @tparam trait the method-trait object
 * @return rtti::MetaMethod
 */
template <typename trait>
rtti::MetaMethod createMethod()
{
  std::vector<std::string_view> names;
  std::ranges::copy(trait::argumentNames | std::views::reverse, std::back_inserter(names));
  names.insert(names.begin(), "this");
  return rtti::MetaMethod {
    trait::function_ptr, trait::name, trait::result_type, names,
    rtti::MetaMethod::Mode::MEMBER + (trait::is_virtual ? rtti::MetaMethod::Mode::VIRTUAL : 0x0)
      + (trait::is_pure_virtual ? rtti::MetaMethod::Mode::PURE_VIRTUAL : 0x0)
      + (trait::is_const ? rtti::MetaMethod::Mode::CONST : 0x0)
      + (trait::is_override ? rtti::MetaMethod::Mode::OVERRIDE : 0x0)
      + (trait::is_final ? rtti::MetaMethod::Mode::FINAL : 0x0)
  };
}

/**
 * @brief Create a Method object
 *
 * Create a RunTime Type Introspection object for a class member function
 *
 * \note the compiler will throw a meta exception if the index does not represent a publically
 * accessible non-static member function.
 *
 * @tparam T the class the method is part of
 * @tparam I the index of the method inside the class
 * @return rtti::MetaMethod
 */
template <typename T, size_t I>
rtti::MetaMethod createMethod()
{
  return createMethod<rtti::details::method_traits<T, I>>();
}
