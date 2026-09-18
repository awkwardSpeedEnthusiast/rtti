#include "testclass.hpp"

namespace my::test::ns
{
MyTestClass::MyTestClass(std::string name)
  : _name { std::move(name) }
{
}

std::string MyTestClass::name() const
{
  return _name;
}

void MyTestClass::setString(std::string const& name)
{
  _name = name;
}

std::span<float> MyTestClass::values()
{
  return _values;
}

std::span<float const> MyTestClass::values() const
{
  return _values;
}

} // namespace my::test::ns
