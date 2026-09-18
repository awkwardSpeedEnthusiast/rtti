#pragma once

#include <span>
#include <string>
#include <vector>

namespace my::test::ns
{
class MyTestClass
{
public:
  MyTestClass(std::string name);

  std::string name() const;
  void setString(std::string const& name);

  std::span<float> values();
  std::span<float const> values() const;

private:
  std::string _name;
  std::vector<float> _values;
};

} // namespace my::test::ns
