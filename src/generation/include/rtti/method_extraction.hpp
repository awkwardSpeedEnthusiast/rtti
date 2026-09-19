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
  using M = rtti::MetaMethod::Mode;
  std::vector<std::string_view> names;
  std::ranges::copy(trait::argumentNames | std::views::reverse, std::back_inserter(names));
  names.insert(names.begin(), "this");
  return rtti::MetaMethod { trait::function_ptr, trait::name, trait::result_type, names,
                            M::MEMBER | (trait::is_virtual ? M::VIRTUAL : M::NONE)
                              | (trait::is_pure_virtual ? M::PURE_VIRTUAL : M::NONE)
                              | (trait::is_const ? M::CONST : M::NONE)
                              | (trait::is_override ? M::OVERRIDE : M::NONE)
                              | (trait::is_final ? M::FINAL : M::NONE) };
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
