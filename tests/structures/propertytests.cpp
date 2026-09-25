#include "rtti/metaproperty.hpp"

#include <exception>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

MATCHER_P(AnyIs, p, "")
{
  try {
    *result_listener << "values differ: Expected: " << p << "\n";
    return std::any_cast<decltype(p)>(arg) == p;
  } catch (std::exception& e) {
    *result_listener << "Caught exception: " << e.what();
    return false;
  }
}

namespace
{
class TestPropertyClass
{
public:
  MOCK_METHOD(int, getValue, (), (const));
  MOCK_METHOD(void, setValue, (int v), ());
  MOCK_METHOD(void, valueChanged, (int v));
};
} // namespace

using rtti::MetaMethod;
using rtti::MetaProperty;
using testing::ElementsAreArray;
using testing::Return;
using M = rtti::MetaMethod::Mode;
using namespace std::string_literals;

TEST(PropertyTests, constantProperty)
{
  MetaProperty p { "value", "a value description", true,
                   MetaMethod(&TestPropertyClass::getValue, "getValue", "int", { "this" }) };
  EXPECT_TRUE(p.readOnly());
  EXPECT_TRUE(p.constant());
  EXPECT_FALSE(p.setter().has_value());
  EXPECT_FALSE(p.notifier().has_value());

  TestPropertyClass c;
  EXPECT_CALL(c, getValue()).WillOnce(Return(123));
  EXPECT_THAT(p.value(&c), AnyIs(int(123)));

  EXPECT_FALSE(p.setValue(&c, 3));
}

TEST(PropertyTests, readOnlyProperty)
{
  MetaProperty p { "value", "a value description", true,
                   MetaMethod(&TestPropertyClass::getValue, "getValue", "int", { "this" }),
                   MetaMethod(&TestPropertyClass::valueChanged, "valueChanged", "void",
                              { "this", "v" }) };
  EXPECT_TRUE(p.readOnly());
  EXPECT_FALSE(p.constant());
  EXPECT_FALSE(p.setter().has_value());
  EXPECT_TRUE(p.notifier().has_value());

  TestPropertyClass c;
  EXPECT_CALL(c, getValue()).WillOnce(Return(123));
  EXPECT_THAT(p.value(&c), AnyIs(int(123)));

  EXPECT_FALSE(p.setValue(&c, 3));
}

TEST(PropertyTests, readWriteProperty)
{
  MetaProperty p { "value",
                   "a value description",
                   true,
                   MetaMethod(&TestPropertyClass::getValue, "getValue", "int", { "this" }),
                   MetaMethod(&TestPropertyClass::setValue, "setValue", "void", { "this", "v" }),
                   MetaMethod(&TestPropertyClass::valueChanged, "valueChanged", "void",
                              { "this", "v" }) };
  EXPECT_FALSE(p.readOnly());
  EXPECT_FALSE(p.constant());
  EXPECT_TRUE(p.setter().has_value());
  EXPECT_TRUE(p.notifier().has_value());

  TestPropertyClass c;
  EXPECT_CALL(c, getValue()).WillOnce(Return(123));
  EXPECT_THAT(p.value(&c), AnyIs(int(123)));

  EXPECT_CALL(c, setValue(3));
  EXPECT_TRUE(p.setValue(&c, 3));
}
