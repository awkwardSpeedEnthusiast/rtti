#pragma once

#include "rtti/details/classtrait.hpp"
#include "rtti/property_extraction.hpp"

#include "rtti/metaclass.hpp"
#include "rtti/metamethod.hpp"
#include "rtti/metaproperty.hpp"

#include <meta>
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
    auto methodArray = rtti::details::get_method_infos<T>()
                     | std::views::transform([](auto const& i) { return MetaMethod { i }; })
                     | std::ranges::to<std::vector>();
    auto propertyArray = rtti::get_properties_from_tuple<typename trait::property_tuple>()
                       | std::ranges::to<std::vector>();
    auto bases = rtti::details::get_base_list<^^T>()
               | std::views::transform([](auto const& i) { return i.creator(); })
               | std::ranges::to<std::vector>();
    return rtti::MetaClass { trait::type_name, methodArray, propertyArray, bases };
  }();
  return &theClass;
}

} // namespace rtti
