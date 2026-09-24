#include "data/methods.hpp"
#include "rtti/method_extraction.hpp"

#include "data/inheritance.hpp"
#include "data/testclass.hpp"

#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::ElementsAreArray;
using testing::IsEmpty;

using namespace std::string_literals;

TEST(MethodGenerationTest, publicMethods_properties_MyTestClass)
{
  constexpr auto a = std::meta::access_context::unprivileged();
  constexpr auto c = ^^my::test::ns::MyTestClass;
  auto nameMethod = createMethod<std::meta::members_of(c, a)[1]>();

  EXPECT_EQ(nameMethod.name(), "name");
  EXPECT_EQ(nameMethod.returnType(), "std::__cxx11::basic_string<char>");
  EXPECT_EQ(nameMethod.description(), "");
  EXPECT_THAT(nameMethod.argumentNames(), ElementsAreArray({ "this" }));
  EXPECT_TRUE(nameMethod.isConst());
  EXPECT_FALSE(nameMethod.isVirtual());
  EXPECT_FALSE(nameMethod.isPureVirtual());
  EXPECT_FALSE(nameMethod.isOverride());
  EXPECT_FALSE(nameMethod.isFinal());
  EXPECT_TRUE(nameMethod.isMember());

  auto setNameMethod = createMethod<std::meta::members_of(c, a)[2]>();

  EXPECT_EQ(setNameMethod.name(), "setString");
  EXPECT_EQ(setNameMethod.returnType(), "void");
  EXPECT_EQ(setNameMethod.description(), "");
  EXPECT_THAT(setNameMethod.argumentNames(), ElementsAreArray({ "this", "name" }));
  EXPECT_FALSE(setNameMethod.isConst());
  EXPECT_FALSE(setNameMethod.isVirtual());
  EXPECT_FALSE(setNameMethod.isPureVirtual());
  EXPECT_FALSE(setNameMethod.isOverride());
  EXPECT_FALSE(setNameMethod.isFinal());
  EXPECT_TRUE(setNameMethod.isMember());

  auto value1Method = createMethod<std::meta::members_of(c, a)[3]>();

  EXPECT_EQ(value1Method.name(), "values");
  EXPECT_EQ(value1Method.returnType(), "std::span<float>");
  EXPECT_EQ(value1Method.description(), "");
  EXPECT_THAT(value1Method.argumentNames(), ElementsAreArray({ "this" }));
  EXPECT_FALSE(value1Method.isConst());
  EXPECT_FALSE(value1Method.isVirtual());
  EXPECT_FALSE(value1Method.isPureVirtual());
  EXPECT_FALSE(value1Method.isOverride());
  EXPECT_FALSE(value1Method.isFinal());
  EXPECT_TRUE(value1Method.isMember());

  auto value2Method = createMethod<std::meta::members_of(c, a)[4]>();

  EXPECT_EQ(value2Method.name(), "values");
  EXPECT_EQ(value2Method.returnType(), "std::span<const float>");
  EXPECT_EQ(value2Method.description(), "");
  EXPECT_THAT(value2Method.argumentNames(), ElementsAreArray({ "this" }));
  EXPECT_TRUE(value2Method.isConst());
  EXPECT_FALSE(value2Method.isVirtual());
  EXPECT_FALSE(value2Method.isPureVirtual());
  EXPECT_FALSE(value2Method.isOverride());
  EXPECT_FALSE(value2Method.isFinal());
  EXPECT_TRUE(value2Method.isMember());
}

TEST(MethodGenerationTest, invokeMethods_MyTestClass)
{
  constexpr auto a = std::meta::access_context::unprivileged();
  constexpr auto c = ^^my::test::ns::MyTestClass;

  auto nameMethod = createMethod<std::meta::members_of(c, a)[1]>();
  auto setNameMethod = createMethod<std::meta::members_of(c, a)[2]>();
  auto value1Method = createMethod<std::meta::members_of(c, a)[3]>();
  auto value2Method = createMethod<std::meta::members_of(c, a)[4]>();

  my::test::ns::MyTestClass object { "foo" };

  EXPECT_EQ(std::any_cast<std::string>(nameMethod.invoke({ &object })), "foo");

  setNameMethod.invoke({ &object, "bar"s });

  EXPECT_EQ(std::any_cast<std::string>(nameMethod.invoke({ &object })), "bar");
}

TEST(MethodGenerationTest, methodTypes_base)
{
  constexpr auto a = std::meta::access_context::unprivileged();
  constexpr auto c = ^^BaseClass;
  {
    auto m = createMethod<std::meta::members_of(c, a)[0]>();
    EXPECT_EQ(m.name(), "abstract_const_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_TRUE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_TRUE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[1]>();
    EXPECT_EQ(m.name(), "virtual_const_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_TRUE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[2]>();
    EXPECT_EQ(m.name(), "abstract_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_FALSE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_TRUE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[3]>();
    EXPECT_EQ(m.name(), "virtual_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_FALSE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[4]>();
    EXPECT_EQ(m.name(), "const_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_TRUE(m.isConst());
    EXPECT_FALSE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[5]>();
    EXPECT_EQ(m.name(), "method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_FALSE(m.isConst());
    EXPECT_FALSE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
}

TEST(MethodGenerationTest, methodTypes_derived)
{
  constexpr auto a = std::meta::access_context::unprivileged();
  constexpr auto c = ^^DerivedClass;
  {
    auto m = createMethod<std::meta::members_of(c, a)[0]>();
    EXPECT_EQ(m.name(), "abstract_const_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_TRUE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[1]>();
    EXPECT_EQ(m.name(), "virtual_const_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_TRUE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[2]>();
    EXPECT_EQ(m.name(), "abstract_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_FALSE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[3]>();
    EXPECT_EQ(m.name(), "virtual_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_FALSE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[4]>();
    EXPECT_EQ(m.name(), "const_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_TRUE(m.isConst());
    EXPECT_FALSE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
}

TEST(MethodGenerationTest, methodTypes_derivedWithFinal)
{
  constexpr auto a = std::meta::access_context::unprivileged();
  constexpr auto c = ^^DerivedClassWithFinal;
  {
    auto m = createMethod<std::meta::members_of(c, a)[0]>();
    EXPECT_EQ(m.name(), "abstract_const_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_TRUE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_TRUE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[1]>();
    EXPECT_EQ(m.name(), "virtual_const_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_TRUE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_TRUE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[2]>();
    EXPECT_EQ(m.name(), "abstract_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_FALSE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_TRUE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[3]>();
    EXPECT_EQ(m.name(), "virtual_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_FALSE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_TRUE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
}

TEST(MethodGenerationTest, methodTypes_final)
{
  constexpr auto a = std::meta::access_context::unprivileged();
  constexpr auto c = ^^FinalClass;
  {
    auto m = createMethod<std::meta::members_of(c, a)[0]>();
    EXPECT_EQ(m.name(), "abstract_const_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_TRUE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[1]>();
    EXPECT_EQ(m.name(), "virtual_const_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_TRUE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[2]>();
    EXPECT_EQ(m.name(), "abstract_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_FALSE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
  {
    auto m = createMethod<std::meta::members_of(c, a)[3]>();
    EXPECT_EQ(m.name(), "virtual_method");
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_EQ(m.description(), "");
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this" }));
    EXPECT_FALSE(m.isConst());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_TRUE(m.isOverride());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isMember());
  }
}

TEST(MethodGenerationTest, ExcessiveArgumentsMethod)
{
  constexpr auto a = std::meta::access_context::unprivileged();
  constexpr auto c = ^^ExcessiveArgument;
  auto m = createMethod<std::meta::members_of(c, a)[0]>();
  EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "this", "one", "two", "three", "four", "five",
                                                    "six", "seven", "eight", "nine", "ten" }));
}
