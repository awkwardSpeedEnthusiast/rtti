#include "rtti/metaclass.hpp"
#include "rtti/metamethod.hpp"
#include "rtti/metaproperty.hpp"
#include <string_view>

namespace rtti
{

MetaClass::MetaClass(std::string_view name, std::vector<MetaMethod> methods,
                     std::vector<MetaProperty> properties)
  : _methods { std::move(methods) }
  , _properties { std::move(properties) }
  , _name { name }
{
}

std::string_view MetaClass::classname() const
{
  return _name;
}

template <typename T>
std::span<T> MetaClass::range()
{
  if constexpr (std::same_as<T, MetaMethod>) {
    return _methods;
  } else if constexpr (std::same_as<T, MetaProperty>) {
    return _properties;
  }
}
template std::span<MetaMethod> MetaClass::range<MetaMethod>();
template std::span<MetaProperty> MetaClass::range<MetaProperty>();

template <typename T>
std::span<T const> MetaClass::range() const
{
  if constexpr (std::same_as<T, MetaMethod>) {
    return _methods;
  } else if constexpr (std::same_as<T, MetaProperty>) {
    return _properties;
  }
}
template std::span<MetaMethod const> MetaClass::range<MetaMethod>() const;
template std::span<MetaProperty const> MetaClass::range<MetaProperty>() const;

} // namespace rtti
