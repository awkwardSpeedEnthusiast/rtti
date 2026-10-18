#include "rtti/metaclass.hpp"
#include "rtti/metamethod.hpp"
#include "rtti/metaproperty.hpp"
#include <string_view>

namespace rtti
{

MetaClass::MetaClass(std::string_view name, std::string nameSpace, std::vector<MetaMethod> methods,
                     std::vector<MetaProperty> properties, std::vector<MetaClass*> bases)
  : _methods { std::move(methods) }
  , _properties { std::move(properties) }
  , _bases { std::move(bases) }
  , _name { name }
  , _namespace { nameSpace }
{
}

std::string_view MetaClass::classname() const
{
  return _name;
}

std::string_view MetaClass::inNamespace() const
{
  return _namespace;
}

std::string MetaClass::completeClassname() const
{
  return _namespace + "::" + _name;
}

bool MetaClass::inherits(MetaClass const& other) const
{
  return std::ranges::find_if(_bases,
                              [&other](auto* b) { return *b == other || b->inherits(other); })
      != _bases.end();
}

template <typename T>
std::span<T> MetaClass::range()
{
  if constexpr (std::same_as<T, MetaMethod>) {
    return _methods;
  } else if constexpr (std::same_as<T, MetaProperty>) {
    return _properties;
  } else if constexpr (std::same_as<T, MetaClass*>) {
    return _bases;
  }
}
template std::span<MetaMethod> MetaClass::range<MetaMethod>();
template std::span<MetaProperty> MetaClass::range<MetaProperty>();
template std::span<MetaClass*> MetaClass::range<MetaClass*>();

template <typename T>
std::span<T const> MetaClass::range() const
{
  if constexpr (std::same_as<T, MetaMethod>) {
    return _methods;
  } else if constexpr (std::same_as<T, MetaProperty>) {
    return _properties;
  } else if constexpr (std::same_as<T, MetaClass*>) {
    return _bases;
  }
}
template std::span<MetaMethod const> MetaClass::range<MetaMethod>() const;
template std::span<MetaProperty const> MetaClass::range<MetaProperty>() const;
template std::span<MetaClass* const> MetaClass::range<MetaClass*>() const;

} // namespace rtti
