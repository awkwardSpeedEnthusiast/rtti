#include "rtti/class_extraction.hpp"

#include "data/classeswithannotations.hpp"
#include "data/inheritance.hpp"
#include "data/testclass.hpp"

#include "rtti/metaclass.hpp"
#include "rtti/metamethod.hpp"
#include "rtti/metaproperty.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <tuple>

using testing::ElementsAreArray;
using testing::IsEmpty;
using testing::SizeIs;

using rtti::createMethod;

TEST(ClassGenerationTests, basic)
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
  EXPECT_THAT(cls->range<rtti::MetaProperty>(), IsEmpty());
  EXPECT_THAT(cls->range<rtti::MetaClass*>(), IsEmpty());
}

TEST(ClassGenerationTests, BaseClass)
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
  EXPECT_THAT(cls->range<rtti::MetaProperty>(), IsEmpty());
  EXPECT_THAT(cls->range<rtti::MetaClass*>(), IsEmpty());
}

TEST(ClassGenerationTests, DerivedClass)
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
  EXPECT_THAT(cls->range<rtti::MetaProperty>(), IsEmpty());
  EXPECT_THAT(cls->range<rtti::MetaClass*>(), ElementsAreArray({ rtti::createClass<BaseClass>() }));
}

TEST(ClassGenerationTests, DerivedClassWithFinalFunction)
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
  EXPECT_THAT(cls->range<rtti::MetaProperty>(), IsEmpty());
  EXPECT_THAT(cls->range<rtti::MetaClass*>(), ElementsAreArray({ rtti::createClass<BaseClass>() }));
}

TEST(ClassGenerationTests, FinalClass)
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
  EXPECT_THAT(cls->range<rtti::MetaProperty>(), IsEmpty());
  EXPECT_THAT(cls->range<rtti::MetaClass*>(), ElementsAreArray({ rtti::createClass<BaseClass>() }));
}

TEST(ClassGenerationTests, Multiple)
{
  using T = Multiple;
  auto cls = rtti::createClass<T>();
  EXPECT_EQ(cls->classname(), "Multiple");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), IsEmpty());
  EXPECT_THAT(cls->range<rtti::MetaProperty>(), IsEmpty());
  EXPECT_THAT(cls->range<rtti::MetaClass*>(),
              ElementsAreArray({ rtti::createClass<BaseClass>(), rtti::createClass<Other>() }));
}

TEST(ClassGenerationTests, OneProperty)
{
  using T = test::TestingClass;
  constexpr auto c = ^^T;
  constexpr auto a = std::meta::access_context::unprivileged();
  auto cls = rtti::createClass<T>();
  EXPECT_EQ(cls->classname(), "TestingClass");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(3));
  EXPECT_THAT(cls->range<rtti::MetaMethod>(),
              ElementsAreArray({ (createMethod<std::meta::members_of(c, a)[0]>()),
                                 createMethod<std::meta::members_of(c, a)[1]>(),
                                 createMethod<std::meta::members_of(c, a)[2]>() }));
  using P = rtti::property_list<test::TestingClass>::property_tuple;
  EXPECT_THAT(cls->range<rtti::MetaProperty>(), SizeIs(1));
  EXPECT_THAT(cls->range<rtti::MetaProperty>(),
              ElementsAreArray({ (rtti::createProperty<std::tuple_element_t<0, P>>()) }));
  EXPECT_THAT(cls->range<rtti::MetaClass*>(), IsEmpty());
}

TEST(ClassGenerationTests, TwoProperties)
{
  using T = test::TwoProperties;
  constexpr auto c = ^^T;
  constexpr auto a = std::meta::access_context::unprivileged();
  auto cls = rtti::createClass<T>();
  EXPECT_EQ(cls->classname(), "TwoProperties");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(7));
  EXPECT_THAT(cls->range<rtti::MetaMethod>(),
              ElementsAreArray({ (createMethod<std::meta::members_of(c, a)[0]>()),
                                 createMethod<std::meta::members_of(c, a)[1]>(),
                                 createMethod<std::meta::members_of(c, a)[2]>(),
                                 createMethod<std::meta::members_of(c, a)[3]>(),
                                 createMethod<std::meta::members_of(c, a)[4]>(),
                                 createMethod<std::meta::members_of(c, a)[5]>(),
                                 createMethod<std::meta::members_of(c, a)[6]>() }));
  using P = rtti::property_list<test::TwoProperties>::property_tuple;
  EXPECT_THAT(cls->range<rtti::MetaProperty>(), SizeIs(2));
  EXPECT_THAT(cls->range<rtti::MetaProperty>(),
              ElementsAreArray({ (rtti::createProperty<std::tuple_element_t<0, P>>()),
                                 (rtti::createProperty<std::tuple_element_t<1, P>>()) }));

  EXPECT_THAT(cls->range<rtti::MetaClass*>(), IsEmpty());
}
