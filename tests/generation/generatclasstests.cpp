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
  auto cls = rtti::createClass<my::test::ns::MyTestClass>();
  EXPECT_EQ(cls->classname(), "MyTestClass");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(4));
  EXPECT_THAT(cls->range<rtti::MetaMethod>(),
              ElementsAreArray({ (createMethod<my::test::ns::MyTestClass, 1>()),
                                 createMethod<my::test::ns::MyTestClass, 2>(),
                                 createMethod<my::test::ns::MyTestClass, 3>(),
                                 createMethod<my::test::ns::MyTestClass, 4>() }));
}

TEST(ClassGeenerationTests, BaseClass)
{
  using T = BaseClass;
  auto cls = rtti::createClass<T>();
  EXPECT_EQ(cls->classname(), "BaseClass");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(6));
  EXPECT_THAT(
    cls->range<rtti::MetaMethod>(),
    ElementsAreArray({ (createMethod<T, 0>()), createMethod<T, 1>(), createMethod<T, 2>(),
                       createMethod<T, 3>(), createMethod<T, 4>(), createMethod<T, 5>() }));
  // currently no static member functions extracted.
}

TEST(ClassGeenerationTests, DerivedClass)
{
  using T = DerivedClass;
  auto cls = rtti::createClass<T>();
  EXPECT_EQ(cls->classname(), "DerivedClass");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(5));
  EXPECT_THAT(cls->range<rtti::MetaMethod>(),
              ElementsAreArray({ (createMethod<T, 0>()), createMethod<T, 1>(), createMethod<T, 2>(),
                                 createMethod<T, 3>(), createMethod<T, 4>() }));
}

TEST(ClassGeenerationTests, DerivedClassWithFinalFunction)
{
  using T = DerivedClassWithFinal;
  auto cls = rtti::createClass<T>();
  EXPECT_EQ(cls->classname(), "DerivedClassWithFinal");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(4));
  EXPECT_THAT(cls->range<rtti::MetaMethod>(),
              ElementsAreArray({ (createMethod<T, 0>()), createMethod<T, 1>(), createMethod<T, 2>(),
                                 createMethod<T, 3>() }));
}

TEST(ClassGeenerationTests, FinalClass)
{
  using T = FinalClass;
  auto cls = rtti::createClass<T>();
  EXPECT_EQ(cls->classname(), "FinalClass");
  EXPECT_THAT(cls->range<rtti::MetaMethod>(), SizeIs(4));
  EXPECT_THAT(cls->range<rtti::MetaMethod>(),
              ElementsAreArray({ (createMethod<T, 0>()), createMethod<T, 1>(), createMethod<T, 2>(),
                                 createMethod<T, 3>() }));
}
