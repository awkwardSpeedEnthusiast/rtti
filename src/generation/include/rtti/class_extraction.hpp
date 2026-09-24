#pragma once

#include "rtti/details/classtrait.hpp"

#include "rtti/metaclass.hpp"
#include "rtti/metamethod.hpp"

#include <ranges>
#include <type_traits>

namespace rtti
{
/**
 * @brief Create a MetaClass object
 *
 * Creates a \a rtti::MetaClass object from a class type. Creates the same class description only
 * once and returns the same object for each invocation.
 *
 * @tparam T the type to create the description for,
 * @return rtti::MetaClass*
 */
template <typename T>
rtti::MetaClass* createClass()
{
  static rtti::MetaClass theClass = []() {
    using trait = rtti::details::class_traits<T>;
    auto methodArray = rtti::details::get_array_from_tuple<typename trait::method_tuple>()
                     | std::ranges::to<std::vector>();
    return rtti::MetaClass { trait::type_name, methodArray };
  }();
  return &theClass;
}

} // namespace rtti
