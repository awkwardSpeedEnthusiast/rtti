#include "rtti/metamethod.hpp"

#include <any>
#include <array>
#include <concepts>
#include <cstddef>
#include <format>
#include <functional>
#include <memory>
#include <meta>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace rtti
{
MetaMethod::MetaMethod(MetaMethod const& other)
  : _p { other._p->clone() }
  , _name { other._name }
  , _returnType { other._returnType }
  , _arguments { other._arguments }
  , _description { other._description }
  , _modes { other._modes }
{
}

MetaMethod::MetaMethod(MetaMethod&& other)
  : _p { std::move(other._p) }
  , _name { std::move(other._name) }
  , _returnType { std::move(other._returnType) }
  , _arguments { std::move(other._arguments) }
  , _description { std::move(other._description) }
  , _modes { std::move(other._modes) }
{
}

MetaMethod& MetaMethod::operator=(MetaMethod const& other)
{
  auto copy = other._p->clone();
  std::swap(_p, copy);
  _name = other._name;
  _arguments = other._arguments;
  _returnType = other._returnType;
  _description = other._description;
  _modes = other._modes;
  return *this;
}

MetaMethod& MetaMethod::operator=(MetaMethod&& other)
{
  std::swap(_p, other._p);
  std::swap(_name, other._name);
  std::swap(_arguments, other._arguments);
  std::swap(_returnType, other._returnType);
  std::swap(_description, other._description);
  std::swap(_modes, other._modes);
  return *this;
}

std::any MetaMethod::invoke(std::vector<std::any> arguments)
{
  return _p->invoke(arguments);
}

std::string_view MetaMethod::name() const
{
  return _name;
}

std::string_view MetaMethod::returnType() const
{
  return _returnType;
}

std::string_view MetaMethod::description() const
{
  return _description;
}

std::span<std::string const> MetaMethod::argumentNames() const
{
  return _arguments;
}

bool MetaMethod::isConst() const
{
  return (_modes & Mode::CONST) != 0;
}

bool MetaMethod::isVirtual() const
{
  return (_modes & Mode::VIRTUAL) != 0;
}

bool MetaMethod::isPureVirtual() const
{
  return (_modes & Mode::PURE_VIRTUAL) != 0;
}

bool MetaMethod::isOverride() const
{
  return (_modes & Mode::OVERRIDE) != 0;
}

bool MetaMethod::isFinal() const
{
  return (_modes & Mode::FINAL) != 0;
}

bool MetaMethod::isMember() const
{
  return (_modes & Mode::MEMBER) != 0;
}

} // namespace rtti
