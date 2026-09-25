#pragma once

#include "rtti/metamethod.hpp"

namespace rtti
{

class MetaProperty
{
public:
  MetaProperty(std::string name, std::string description, bool isTransient, MetaMethod getter);
  MetaProperty(std::string name, std::string description, bool isTransient, MetaMethod getter,
               MetaMethod notifier);
  MetaProperty(std::string name, std::string description, bool isTransient, MetaMethod getter,
               MetaMethod setter, MetaMethod notifier);
  MetaProperty(MetaProperty const& other) = default;
  MetaProperty(MetaProperty&& other) = default;
  MetaProperty& operator=(MetaProperty const& other) = default;
  MetaProperty& operator=(MetaProperty&& other) = default;
  ~MetaProperty() = default;

  std::string_view name() const;
  std::string_view description() const;
  MetaMethod getter() const;
  std::optional<MetaMethod> setter() const;
  std::optional<MetaMethod> notifier() const;
  bool isTransient() const;

  bool readOnly() const;
  bool constant() const;

  std::any value(std::any object);
  bool setValue(std::any object, std::any value);

private:
  std::string _name;
  std::string _description;
  MetaMethod _getter;
  std::optional<MetaMethod> _setter;
  std::optional<MetaMethod> _notifier;
  bool _isTransient;
};
} // namespace rtti