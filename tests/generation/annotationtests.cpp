#include "data/classeswithannotations.hpp"
#include "data/methods.hpp"
#include "data/testclass.hpp"
#include "rtti/details/argumentlist.hpp"
#include "rtti/details/helpers.hpp"
#include "rtti/details/metatools.hpp"
#include "rtti/details/methodlist.hpp"
#include "rtti/details/propertystruct.hpp"
#include "rtti/details/tuplebuilder.hpp"
#include "rtti/property_extraction.hpp"

#include <concepts>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iterator>
#include <meta>
#include <ranges>
#include <tuple>
#include <type_traits>

TEST(AnnotationTests, PropertyStruct)
{
  constexpr auto prop1 = rtti::find_property_definition<test::TestingClass>();
  constexpr auto prop2 = rtti::find_property_definition<my::test::ns::MyTestClass>();
  EXPECT_TRUE(std::meta::is_type(prop1));
  EXPECT_TRUE(std::meta::is_type(prop2));
  EXPECT_FALSE(std::meta::is_void_type(prop1));
  EXPECT_TRUE(std::meta::is_void_type(prop2));
  EXPECT_EQ(std::meta::identifier_of(prop1), "properties");
}

TEST(AnnotationTests, PropertyStructExtraction)
{
  using P = rtti::property_struct<test::TestingClass>;
  if constexpr (!P::value) {
    FAIL() << "no property struct found";
  }
  EXPECT_TRUE(P::is_valid);
}

TEST(AnnotationTests, PropertyStructExtraction_failed)
{
  using P = rtti::property_struct<my::test::ns::MyTestClass>;
  if constexpr (P::value) {
    FAIL() << "There should not be a property struct";
  }
}

TEST(AnnotationTests, PropertyExtraction)
{
  using P = rtti::property_struct<test::TestingClass>;
  using sing = rtti::single_prop_t<std::meta::enumerators_of(P::name_e)[0]>;
  EXPECT_TRUE(P::is_valid);
  static_assert(std::meta::has_parent(P::name_e));
  constexpr auto v1 = std::meta::enumerators_of(P::name_e)[0];
  EXPECT_TRUE(sing::val == [:v1:]);
  static_assert(sing::parent_enum == P::name_e);
  static_assert(sing::clazz == ^^test::TestingClass);
  static_assert(sing::has_getter);
  static_assert(sing::has_setter);
  static_assert(sing::has_notifier);
  static_assert(sing::is_transient);
}

TEST(AnnotationTests, PropertyListExtraction)
{
  using P_list = rtti::property_list<test::TestingClass>;
  static_assert(P_list::value);
  static_assert(std::tuple_size_v<P_list::property_tuple> == 1);

  using p1 = std::tuple_element_t<0, P_list::property_tuple>;
  EXPECT_EQ(std::meta::identifier_of(p1::getter), "getValue");
  EXPECT_EQ(std::meta::identifier_of(p1::setter), "setValue");
  EXPECT_EQ(std::meta::identifier_of(p1::notifier), "connectValueChange");
}

TEST(AnnotationTests, PropertyListExtraction2)
{
  using P_list = rtti::property_list<test::TwoProperties>;
  static_assert(P_list::value);
  static_assert(std::tuple_size_v<P_list::property_tuple> == 2);

  using p1 = std::tuple_element_t<0, P_list::property_tuple>;
  EXPECT_EQ(std::meta::identifier_of(p1::getter), "getValue1");
  EXPECT_EQ(std::meta::identifier_of(p1::setter), "setValue1");
  EXPECT_EQ(std::meta::identifier_of(p1::notifier), "connectValue1Change");

  using p2 = std::tuple_element_t<1, P_list::property_tuple>;
  EXPECT_EQ(std::meta::identifier_of(p2::getter), "getValue2");
  EXPECT_EQ(std::meta::identifier_of(p2::setter), "setValue2");
  EXPECT_EQ(std::meta::identifier_of(p2::notifier), "connectValue2Change");
}

TEST(AnnotationTests, BadProperty)
{
  using list_noGetter = rtti::property_list<test::NoGetter>;

  static_assert(list_noGetter::value);
  static_assert(std::tuple_size_v<list_noGetter::property_tuple> == 0);

  using list_badConstness = rtti::property_list<test::BadConstness>;
  static_assert(list_badConstness::value);
  static_assert(std::tuple_size_v<list_badConstness::property_tuple> == 2);

  using p1 = std::tuple_element_t<0, list_badConstness::property_tuple>;
  EXPECT_EQ(std::meta::identifier_of(p1::getter), "getValue2");
  EXPECT_FALSE(p1::has_setter);
  EXPECT_EQ(std::meta::identifier_of(p1::notifier), "connectValue2Change");

  using p2 = std::tuple_element_t<1, list_badConstness::property_tuple>;
  EXPECT_EQ(std::meta::identifier_of(p2::getter), "getValue3");
  EXPECT_EQ(std::meta::identifier_of(p2::setter), "setValue3");
  EXPECT_FALSE(p2::has_notifier);
}
