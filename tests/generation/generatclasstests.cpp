#include "rtti/class_extraction.hpp"

#include "data/inheritance.hpp"
#include "data/testclass.hpp"

#include "rtti/metamethod.hpp"
#include "rtti/method_extraction.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::ElementsAreArray;
using testing::SizeIs;

TEST(ClassGeenerationTests, basic)
{
  constexpr auto c = ^^my::test::ns::MyTestClass;
  constexpr auto a = std::meta::access_context::unprivileged();
  auto cls = rtti::createClass<my::test::ns::MyTestClass>();
  EXPECT_EQ(cls->classname(), "MyTestClass");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(4));
  EXPECT_THAT(cls->range<rtti::MetaMethod>(),
              ElementsAreArray({ (createMethod<std::meta::members_of(c, a)[1]>()),
                                 createMethod<std::meta::members_of(c, a)[2]>(),
                                 createMethod<std::meta::members_of(c, a)[3]>(),
                                 createMethod<std::meta::members_of(c, a)[4]>() }));
}

TEST(ClassGeenerationTests, BaseClass)
{
  using T = BaseClass;
  constexpr auto c = ^^T;
  constexpr auto a = std::meta::access_context::unprivileged();
  auto cls = rtti::createClass<T>();
  EXPECT_EQ(cls->classname(), "BaseClass");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(6));
  EXPECT_THAT(cls->range<rtti::MetaMethod>(),
              ElementsAreArray({ (createMethod<std::meta::members_of(c, a)[0]>()),
                                 createMethod<std::meta::members_of(c, a)[1]>(),
                                 createMethod<std::meta::members_of(c, a)[2]>(),
                                 createMethod<std::meta::members_of(c, a)[3]>(),
                                 createMethod<std::meta::members_of(c, a)[4]>(),
                                 createMethod<std::meta::members_of(c, a)[5]>() }));
  // currently no static member functions extracted.
}

TEST(ClassGeenerationTests, DerivedClass)
{
  using T = DerivedClass;
  constexpr auto c = ^^T;
  constexpr auto a = std::meta::access_context::unprivileged();
  auto cls = rtti::createClass<T>();
  EXPECT_EQ(cls->classname(), "DerivedClass");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(5));
  EXPECT_THAT(cls->range<rtti::MetaMethod>(),
              ElementsAreArray({ (createMethod<std::meta::members_of(c, a)[0]>()),
                                 createMethod<std::meta::members_of(c, a)[1]>(),
                                 createMethod<std::meta::members_of(c, a)[2]>(),
                                 createMethod<std::meta::members_of(c, a)[3]>(),
                                 createMethod<std::meta::members_of(c, a)[4]>() }));
}

TEST(ClassGeenerationTests, DerivedClassWithFinalFunction)
{
  using T = DerivedClassWithFinal;
  constexpr auto c = ^^T;
  constexpr auto a = std::meta::access_context::unprivileged();
  auto cls = rtti::createClass<T>();
  EXPECT_EQ(cls->classname(), "DerivedClassWithFinal");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(4));
  EXPECT_THAT(cls->range<rtti::MetaMethod>(),
              ElementsAreArray({ (createMethod<std::meta::members_of(c, a)[0]>()),
                                 createMethod<std::meta::members_of(c, a)[1]>(),
                                 createMethod<std::meta::members_of(c, a)[2]>(),
                                 createMethod<std::meta::members_of(c, a)[3]>() }));
}

TEST(ClassGeenerationTests, FinalClass)
{
  using T = FinalClass;
  constexpr auto c = ^^T;
  constexpr auto a = std::meta::access_context::unprivileged();
  auto cls = rtti::createClass<T>();
  EXPECT_EQ(cls->classname(), "FinalClass");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(4));
  EXPECT_THAT(cls->range<rtti::MetaMethod>(),
              ElementsAreArray({ (createMethod<std::meta::members_of(c, a)[0]>()),
                                 createMethod<std::meta::members_of(c, a)[1]>(),
                                 createMethod<std::meta::members_of(c, a)[2]>(),
                                 createMethod<std::meta::members_of(c, a)[3]>() }));
}
