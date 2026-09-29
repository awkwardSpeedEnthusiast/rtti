#pragma once

#include "rtti/details/methodtools.hpp"

#include <any>
#include <meta>
#include <vector>

namespace rtti
{

/**
 * @brief Class describing a Method
 *
 * This object provides description of a method. This can be:
 *  * a free function
 *  * a class member function
 *  * a const class member function
 *  * a static member function
 *
 * Along with string properties like name, return type, constness, virtual,
 * etc. it provides the possibility to invoke the method by providing a vector
 * of appropriate std::any objects.
 * With the current version of the MetaMethod, the types need to fit exactly.
 * Looking into accepting convertible types.
 *
 * The class uses the type-erasure pattern to encapsule the invocation
 * functionality.
 */
class MetaMethod
{
public:
  MetaMethod(details::f_info const& d)
    : _name { d.name }
    , _returnType { d.returnType }
    , _invoke { d.invoke }
    , _is_static { d.is_static }
    , _is_const { d.is_const }
    , _is_override { d.is_override }
    , _is_virtual { d.is_virtual }
    , _is_pure_virtual { d.is_pure_virtual }
    , _is_final { d.is_final }
    , _is_member { d.is_member }
    , _is_lambda { d.is_lambda }
  {
    auto list = d.alc();
    _argument_names = std::vector<std::string_view> { list.begin(), list.end() };
    if (_is_member) {
      _argument_names.insert(_argument_names.begin(), "this");
    }
  }

  MetaMethod(MetaMethod const&) = default;
  MetaMethod(MetaMethod&&) = default;
  MetaMethod& operator=(MetaMethod const&) = default;
  MetaMethod& operator=(MetaMethod&&) = default;
  ~MetaMethod() = default;
  auto operator<=>(MetaMethod const&) const = default;
  bool operator==(MetaMethod const&) const = default;
  /**
   * @brief Invoke the method
   *
   * throws std::bad_any_cast if the argument types do not match exactly
   *
   * @param arguments arguments to invoke the method with
   * @return std::any
   */
  std::any invoke(std::vector<std::any> args)
  {
    return _invoke(args);
  }

  /**
   * @brief returns the name of the method
   *
   * @return std::string_view
   */
  std::string_view name() const
  {
    return _name;
  }
  /**
   * @brief returns the name of the return type of the method
   *
   * @return std::string_view
   */
  std::string_view returnType() const
  {
    return _returnType;
  }
  /**
   * @brief returns the description of the method
   *
   * @return std::string_view
   */
  std::string_view description() const
  {
    return _description;
  }
  /**
   * @brief returns the list of argument names
   *
   * @return std::span<std::string const>
   */
  std::vector<std::string_view> argumentNames() const
  {
    return _argument_names;
  }
  /**
   * @brief query whether the method is a static member function
   *
   * @return true
   * @return false
   */
  bool isStatic() const
  {
    return _is_static;
  }
  /**
   * @brief queries constness
   *
   * @return true
   * @return false
   */
  bool isConst() const
  {
    return _is_const;
  }
  /**
   * @brief queries virtual
   *
   * @return true
   * @return false
   */
  bool isVirtual() const
  {
    return _is_virtual;
  }
  /**
   * @brief queries pure virtual
   *
   * @return true
   * @return false
   */
  bool isPureVirtual() const
  {
    return _is_pure_virtual;
  }
  /**
   * @brief queries if overridden
   *
   * @return true
   * @return false
   */
  bool isOverride() const
  {
    return _is_override;
  }
  /**
   * @brief queries if final
   *
   * @return true
   * @return false
   */
  bool isFinal() const
  {
    return _is_final;
  }
  /**
   * @brief queries whether it is a member function
   *
   * @return true
   * @return false
   */
  bool isMember() const
  {
    return _is_member;
  }
  /**
   * @brief queries whether it represents a lambda
   *
   * @return true
   * @return false
   */
  bool isLambda() const
  {
    return _is_lambda;
  }

private:
  std::string_view _name;
  std::string_view _description;
  std::string_view _returnType;
  details::ErasedInvoker _invoke;
  bool _is_static;
  bool _is_const;
  bool _is_override;
  bool _is_virtual;
  bool _is_pure_virtual;
  bool _is_final;
  bool _is_member;
  bool _is_lambda;
  std::vector<std::string_view> _argument_names;
};

/**
 * @brief Create a Method object
 *
 * Create a RunTime Type Introspection object for a class member function
 *
 * \note the compiler will throw a meta exception if the index does not represent a publically
 * accessible member function.
 *
 * @tparam method the reflection of the method in question
 * @return rtti::MetaMethod
 */
template <std::meta::info method>
MetaMethod createMethod()
{
  static rtti::MetaMethod m = [] {
    if constexpr (std::meta::has_parent(method)) {
      if constexpr (std::meta::is_type(std::meta::parent_of(method))) {
        constexpr auto info =
          rtti::details::createFromType<method, typename[:std::meta::parent_of(method):]>();
        return MetaMethod { info };
      } else {
        constexpr auto info = rtti::details::createFromType<method, void>();
        return MetaMethod { info };
      }
    } else {
      constexpr auto info = rtti::details::createFromType<method, void>();
      return MetaMethod { info };
    }
  }();
  return m;
}
} // namespace rtti
