#include "rtti/metamethod.hpp"

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
