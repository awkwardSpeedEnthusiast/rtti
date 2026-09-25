#pragma once
#include "rtti/property_definition.hpp"

#include <functional>
#include <string_view>

namespace[[= 7]] test
{

class[[= false]] TestingClass
{
private:
  struct properties {
    enum class props { value[[= rtti::property::Transient {}]] };
    props myName;
    rtti::property::types myType;
  };

public:
  [[= test::TestingClass::properties { test::TestingClass::properties::props::value,
                                       rtti::property::types::getter }]] int getValue() const
  {
    return 0;
  }
  [[= test::TestingClass::properties { test::TestingClass::properties::props::value,
                                       rtti::property::types::setter }]] void setValue(int v)
  {
    (void) v;
  }
  [[= test::TestingClass::properties { test::TestingClass::properties::props::value,
                                       rtti::property::types::notifier }]] void
    connectValueChange(std::function<void(int)> slot)
  {
    (void) slot;
  }
};

class TwoProperties
{
private:
  struct properties {
    enum class props {
      value1[[= rtti::property::Transient {}]],
      value2,
    };
    props myName;
    rtti::property::types myType;
  };

public:
  [[= test::TwoProperties::properties { test::TwoProperties::properties::props::value1,
                                        rtti::property::types::getter }]] int getValue1() const
  {
    return 0;
  }
  [[= test::TwoProperties::properties { test::TwoProperties::properties::props::value1,
                                        rtti::property::types::setter }]] void setValue1(int v)
  {
    (void) v;
  }
  [[= test::TwoProperties::properties { test::TwoProperties::properties::props::value1,
                                        rtti::property::types::notifier }]] void
    connectValue1Change(std::function<void(int)> slot)
  {
    (void) slot;
  }

  void someOtherMethod(int) const {}

  [[= test::TwoProperties::properties { test::TwoProperties::properties::props::value2,
                                        rtti::property::types::getter }]] int getValue2() const
  {
    return 0;
  }
  [[= test::TwoProperties::properties { test::TwoProperties::properties::props::value2,
                                        rtti::property::types::setter }]] void setValue2(int v)
  {
    (void) v;
  }
  [[= test::TwoProperties::properties { test::TwoProperties::properties::props::value2,
                                        rtti::property::types::notifier }]] void
    connectValue2Change(std::function<void(int)> slot)
  {
    (void) slot;
  }
};

class[[= false]] NoGetter
{
private:
  struct properties {
    enum class props { value };
    props myName;
    rtti::property::types myType;
  };

public:
  [[= test::NoGetter::properties { test::NoGetter::properties::props::value,
                                   rtti::property::types::setter }]] void setValue(int v)
  {
    (void) v;
  }
  [[= test::NoGetter::properties { test::NoGetter::properties::props::value,
                                   rtti::property::types::notifier }]] void
    connectValueChange(std::function<void(int)> slot)
  {
    (void) slot;
  }
};

class[[= false]] BadConstness
{
private:
  struct properties {
    enum class props { value, value2, value3 };
    props myName;
    rtti::property::types myType;
  };

public:
  [[= test::BadConstness::properties { test::BadConstness::properties::props::value,
                                       rtti::property::types::getter }]] int getValue()
  {
    return 0;
  }
  [[= test::BadConstness::properties { test::BadConstness::properties::props::value,
                                       rtti::property::types::setter }]] void setValue(int v)
  {
    (void) v;
  }
  [[= test::BadConstness::properties { test::BadConstness::properties::props::value,
                                       rtti::property::types::notifier }]] void
    connectValueChange(std::function<void(int)> slot)
  {
    (void) slot;
  }

  [[= test::BadConstness::properties { test::BadConstness::properties::props::value2,
                                       rtti::property::types::getter }]] int getValue2() const
  {
    return 0;
  }
  [[= test::BadConstness::properties { test::BadConstness::properties::props::value2,
                                       rtti::property::types::setter }]] void setValue2(int v) const
  {
    (void) v;
  }
  [[= test::BadConstness::properties { test::BadConstness::properties::props::value2,
                                       rtti::property::types::notifier }]] void
    connectValue2Change(std::function<void(int)> slot)
  {
    (void) slot;
  }

  [[= test::BadConstness::properties { test::BadConstness::properties::props::value3,
                                       rtti::property::types::getter }]] int getValue3() const
  {
    return 0;
  }
  [[= test::BadConstness::properties { test::BadConstness::properties::props::value3,
                                       rtti::property::types::setter }]] void setValue3(int v)
  {
    (void) v;
  }
  [[= test::BadConstness::properties { test::BadConstness::properties::props::value3,
                                       rtti::property::types::notifier }]] void
    connectValue3Change(std::function<void(int)> slot) const
  {
    (void) slot;
  }
};

} // namespace test