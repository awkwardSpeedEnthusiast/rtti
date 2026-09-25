#include "rtti/metamethod.hpp"
#include "rtti/metaproperty.hpp"

#include <format>

/**
 * @brief create a string representation of a meta method
 *
 * @param m the method to convert to string
 * @return std::string
 */
std::string to_string(rtti::MetaMethod const& m)
{
  bool first = true;
  std::string args;
  for (auto e : m.argumentNames()) {
    if (!std::exchange(first, false))
      args += ", ";
    args += e;
  }

  return std::format("{} {} {}({}){}{}{}", (m.isVirtual() ? "virtual" : ""), m.returnType(),
                     m.name(), args, m.isConst() ? " const" : "", m.isOverride() ? " override" : "",
                     m.isFinal() ? " final" : "");
}

std::string to_string(rtti::MetaProperty const& m)
{
  return std::format("{} ({}{}) ({})/({})/({})", m.name(), (m.constant() ? "const," : ""),
                     (m.readOnly() ? "read-only," : ""), to_string(m.getter()),
                     m.setter().has_value() ? to_string(*m.setter()) : "",
                     m.notifier().has_value() ? to_string(*m.notifier()) : "");
}
