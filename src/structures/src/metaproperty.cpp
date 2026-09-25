#include "rtti/metaproperty.hpp"
#include <string_view>

namespace rtti
{
MetaProperty::MetaProperty(std::string_view name, std::string_view description, bool isTransient,
                           MetaMethod getter)
  : _name { name }
  , _description { description }
  , _getter { std::move(getter) }
  , _isTransient(isTransient)
{
}

MetaProperty::MetaProperty(std::string_view name, std::string_view description, bool isTransient,
                           MetaMethod getter, MetaMethod notifier)
  : _name { name }
  , _description { description }
  , _getter { std::move(getter) }
  , _notifier { std::move(notifier) }
  , _isTransient(isTransient)
{
}

MetaProperty::MetaProperty(std::string_view name, std::string_view description, bool isTransient,
                           MetaMethod getter, MetaMethod setter, MetaMethod notifier)
  : _name { name }
  , _description { description }
  , _getter { std::move(getter) }
  , _setter { std::move(setter) }
  , _notifier { std::move(notifier) }
  , _isTransient(isTransient)
{
}

std::string_view MetaProperty::name() const
{
  return _name;
}

std::string_view MetaProperty::description() const
{
  return _description;
}

MetaMethod MetaProperty::getter() const
{
  return _getter;
}

std::optional<MetaMethod> MetaProperty::setter() const
{
  return _setter;
}

std::optional<MetaMethod> MetaProperty::notifier() const
{
  return _notifier;
}

bool MetaProperty::isTransient() const
{
  return _isTransient;
}

bool MetaProperty::readOnly() const
{
  return !_setter.has_value();
}

bool MetaProperty::constant() const
{
  return !_notifier.has_value();
}

std::any MetaProperty::value(std::any object)
{
  return _getter.invoke({ object });
}

bool MetaProperty::setValue(std::any object, std::any value)
{
  if (!_setter) {
    return false;
  }
  try {
    _setter->invoke({ object, value });
  } catch (std::exception&) {
    return false;
  }
  return true;
}

} // namespace rtti