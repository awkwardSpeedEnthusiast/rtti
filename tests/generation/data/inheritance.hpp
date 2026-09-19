#pragma once

class BaseClass
{
public:
  virtual int abstract_const_method() const = 0;
  virtual int virtual_const_method() const
  {
    return 1;
  }
  virtual int abstract_method() = 0;
  virtual int virtual_method()
  {
    return 1;
  }
  int const_method() const
  {
    return 1;
  }
  int method()
  {
    return 1;
  }
  static int static_method()
  {
    return 1;
  }
};

class DerivedClass : public BaseClass
{
public:
  int abstract_const_method() const override
  {
    return 1;
  }
  int virtual_const_method() const override
  {
    return 1;
  }
  int abstract_method() override
  {
    return 1;
  }
  int virtual_method() override
  {
    return 1;
  }
  int const_method() const
  {
    return 1;
  }
  static int static_method()
  {
    return 1;
  }
};

class DerivedClassWithFinal : public BaseClass
{
public:
  int abstract_const_method() const final
  {
    return 1;
  }
  int virtual_const_method() const final
  {
    return 1;
  }
  int abstract_method() final
  {
    return 1;
  }
  int virtual_method() final
  {
    return 1;
  }
};

class FinalClass final : public BaseClass
{
public:
  int abstract_const_method() const override
  {
    return 1;
  }
  int virtual_const_method() const override
  {
    return 1;
  }
  int abstract_method() override
  {
    return 1;
  }
  int virtual_method() override
  {
    return 1;
  }
};
