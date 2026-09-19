#include "rtti/metamethod.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <meta>
#include <string>

namespace data
{
void freeVoidFunction(std::string foo)
{
  (void) foo;
}

int freeFunction(std::string const& foo)
{
  (void) foo;
  return 0;
}

auto lambda = [](int i) { return i * 2; };

class TestClass
{
public:
  virtual void virtual_function() {}
  MOCK_METHOD(int, function, ());
  MOCK_METHOD(double, const_function, (), (const));
  static std::string static_function()
  {
    return "foo";
  }
  MOCK_METHOD(int, template_function_M, ());
  template <typename T>
  T template_function()
  {
    if constexpr (std::same_as<T, int>) {
      return template_function_M();
    } else {
      return T {};
    }
  }
  MOCK_METHOD(int, template_const_function_M, (), (const));
  template <typename T>
  T template_const_function() const
  {
    if constexpr (std::same_as<T, int>) {
      return template_const_function_M();
    } else {
      return T {};
    }
  }
};
} // namespace data
using rtti::MetaMethod;
using testing::ElementsAreArray;
using testing::Return;
using M = rtti::MetaMethod::Mode;
using namespace std::string_literals;

TEST(MetaMethodTests, freeFunctions)
{
  {
    auto m = MetaMethod(&data::freeVoidFunction, "foo"s, "void"s, { "foo"s });
    m.invoke({ ""s });
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "foo" }));
    EXPECT_FALSE(m.isConst());
    EXPECT_FALSE(m.isFinal());
    EXPECT_FALSE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_FALSE(m.isMember());
    EXPECT_FALSE(m.isLambda());
  }
  {
    auto m = MetaMethod(&data::freeFunction, "foo", "void", { "foo" });
    std::any result;
    EXPECT_NO_THROW(result = m.invoke({ ""s }));
    EXPECT_EQ(result.type(), typeid(int));
    EXPECT_EQ(std::any_cast<int>(result), 0);
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "foo" }));
    EXPECT_EQ(m.returnType(), "int");
    EXPECT_FALSE(m.isConst());
    EXPECT_FALSE(m.isFinal());
    EXPECT_FALSE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_FALSE(m.isMember());
    EXPECT_FALSE(m.isLambda());
  }
}

// not yet supported
TEST(MetaMethodTests, annonymousFunctions)
{
  {
    auto m = MetaMethod(data::lambda, "lambda", { "i" });
    std::any result;
    EXPECT_NO_THROW(result = m.invoke({ static_cast<int>(0) }));
    EXPECT_EQ(result.type(), typeid(int));
    EXPECT_EQ(std::any_cast<int>(result), 0);
    EXPECT_THAT(m.argumentNames(), ElementsAreArray({ "i" }));
    EXPECT_TRUE(m.isConst());
    EXPECT_FALSE(m.isFinal());
    EXPECT_FALSE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_TRUE(m.isMember());
    EXPECT_TRUE(m.isLambda());
  }
}

TEST(MetaMethodTests, memberFunctions)
{
  {
    auto m = MetaMethod(&data::TestClass::function, "function", "int", { "this" });
    data::TestClass object;
    EXPECT_CALL(object, function()).WillOnce(Return(5));
    std::any result;
    EXPECT_NO_THROW(result = m.invoke({ &object }););
    EXPECT_EQ(result.type(), typeid(int));
    EXPECT_EQ(std::any_cast<int>(result), 5);
    EXPECT_FALSE(m.isConst());
    EXPECT_FALSE(m.isFinal());
    EXPECT_FALSE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_TRUE(m.isMember());
    EXPECT_FALSE(m.isLambda());
  }
  {
    auto m = MetaMethod(&data::TestClass::const_function, "const_function", "double", { "this" });
    data::TestClass object;
    EXPECT_CALL(object, const_function()).WillOnce(Return(42.));
    std::any result;
    EXPECT_NO_THROW(result = m.invoke({ &object }););
    EXPECT_EQ(result.type(), typeid(double));
    EXPECT_EQ(std::any_cast<double>(result), 42.);
    EXPECT_TRUE(m.isConst());
    EXPECT_FALSE(m.isFinal());
    EXPECT_FALSE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_TRUE(m.isMember());
    EXPECT_FALSE(m.isLambda());
  }
  {
    auto m =
      MetaMethod(&data::TestClass::template_function<int>, "template_function", "void", { "this" });
    data::TestClass object;
    EXPECT_CALL(object, template_function_M()).WillOnce(Return(5));
    std::any result;
    EXPECT_NO_THROW(result = m.invoke({ &object }););
    EXPECT_EQ(result.type(), typeid(int));
    EXPECT_EQ(std::any_cast<int>(result), 5);
    EXPECT_EQ(m.returnType(), "int"s);
    EXPECT_FALSE(m.isConst());
    EXPECT_FALSE(m.isFinal());
    EXPECT_FALSE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_TRUE(m.isMember());
    EXPECT_FALSE(m.isLambda());
  }
  {
    auto m = MetaMethod(&data::TestClass::template_const_function<int>, "template_const_function",
                        "void", { "this" });
    data::TestClass object;
    EXPECT_CALL(object, template_const_function_M()).WillOnce(Return(5));
    std::any result;
    EXPECT_NO_THROW(result = m.invoke({ &object }););
    EXPECT_EQ(result.type(), typeid(int));
    EXPECT_EQ(std::any_cast<int>(result), 5);
    EXPECT_EQ(m.returnType(), "int"s);
    EXPECT_TRUE(m.isConst());
    EXPECT_FALSE(m.isFinal());
    EXPECT_FALSE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_TRUE(m.isMember());
    EXPECT_FALSE(m.isLambda());
  }
  {
    // cannot distinguish between static member functions and free functions by type alone, so we
    // have to pass it as mode
    auto m = MetaMethod(&data::TestClass::static_function, "static_function", "std::string", {},
                        M::MEMBER);
    data::TestClass object;
    EXPECT_NO_THROW(m.invoke({ &object }););
    EXPECT_FALSE(m.isConst());
    EXPECT_FALSE(m.isFinal());
    EXPECT_FALSE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_TRUE(m.isMember());
    EXPECT_FALSE(m.isLambda());
  }
  {
    auto m = MetaMethod(&data::TestClass::virtual_function, "virtual_function", "int", { "this" },
                        M::VIRTUAL);
    data::TestClass object;
    EXPECT_NO_THROW(m.invoke({ &object }););
    EXPECT_FALSE(m.isConst());
    EXPECT_FALSE(m.isFinal());
    EXPECT_TRUE(m.isVirtual());
    EXPECT_FALSE(m.isPureVirtual());
    EXPECT_FALSE(m.isOverride());
    EXPECT_TRUE(m.isMember());
    EXPECT_FALSE(m.isLambda());
  }
}
