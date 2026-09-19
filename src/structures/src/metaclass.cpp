#include "rtti/metaclass.hpp"
#include "rtti/metamethod.hpp"
#include <string_view>

namespace rtti
{

MetaClass::MetaClass(std::string_view name, std::vector<MetaMethod> methods)
  : _methods { std::move(methods) }
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
  }
}
template std::span<MetaMethod> MetaClass::range<MetaMethod>();

template <typename T>
std::span<T const> MetaClass::range() const
{
  if constexpr (std::same_as<T, MetaMethod>) {
    return _methods;
  }
}
template std::span<MetaMethod const> MetaClass::range<MetaMethod>() const;

} // namespace rtti
