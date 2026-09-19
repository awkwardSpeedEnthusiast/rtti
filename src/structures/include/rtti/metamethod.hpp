#pragma once

#include "rtti/details/functiontraits.hpp"

#include <any>
#include <concepts>
#include <cstdint>
#include <format>
#include <functional>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/types.h>
#include <utility>
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
  enum Mode : uint8_t {
    NONE = 0x00,
    CONST = 0x01,
    VIRTUAL = 0x02,
    PURE_VIRTUAL = 0x04,
    OVERRIDE = 0x08,
    FINAL = 0x10,
    MEMBER = 0x20,
    LAMBDA = 0x40,
  };

private:
  class Impl
  {
  public:
    virtual ~Impl() = default;
    virtual std::any invoke(std::vector<std::any> const& params) = 0;
    virtual std::unique_ptr<Impl> clone() const = 0;
  };

  template <typename T>
  class Impl_t : public Impl
  {
  public:
    template <typename R, typename... Args>
    explicit Impl_t(R (*f)(Args...))
      : _func(f)
    {
    }
    template <typename R, typename C, typename... Args>
    explicit Impl_t(R (C::*f)(Args...))
      : _func(f)
    {
    }
    template <typename R, typename C, typename... Args>
    explicit Impl_t(R (C::*f)(Args...) const)
      : _func(f)
    {
    }
    std::unique_ptr<Impl> clone() const override
    {
      return std::make_unique<Impl_t<T>>(_func);
    }

    template <int N>
    using NthTypeOf =
      typename std::tuple_element<N, typename detail::function_traits<T>::argument_t>::type;

    template <std::size_t... Indices>
    auto invokeImpl(std::vector<std::any> const& v, std::index_sequence<Indices...>)
    {
      if constexpr (std::same_as<void, typename detail::function_traits<T>::result_t>) {
        std::invoke(_func, std::any_cast<NthTypeOf<Indices>>(v[Indices])...);
      } else {
        return std::invoke(_func, std::any_cast<NthTypeOf<Indices>>(v[Indices])...);
      }
    }

    std::any invoke(std::vector<std::any> const& values) override
    {
      if constexpr (std::same_as<void, typename detail::function_traits<T>::result_t>) {
        invokeImpl(values, std::make_index_sequence<
                             std::tuple_size_v<typename detail::function_traits<T>::argument_t>>());
        return {};
      } else {
        return invokeImpl(values,
                          std::make_index_sequence<
                            std::tuple_size_v<typename detail::function_traits<T>::argument_t>>());
      }
    }

    T _func;
  };

  template <typename T>
  class Impl_lambda_t : public Impl
  {
  public:
    explicit Impl_lambda_t(T&& lambda)
      : _lambda(lambda)
    {
    }
    std::unique_ptr<Impl> clone() const override
    {
      return std::make_unique<Impl_lambda_t<T>>(_lambda);
    }

    template <int N>
    using NthTypeOf =
      typename std::tuple_element<N, typename detail::lambda_traits<T>::argument_t>::type;

    template <std::size_t... Indices>
    auto invokeImpl(std::vector<std::any> const& v, std::index_sequence<Indices...>)
    {
      if constexpr (std::same_as<void, typename detail::lambda_traits<T>::result_t>) {
        std::invoke(_lambda, std::any_cast<NthTypeOf<Indices>>(v[Indices])...);
      } else {
        return std::invoke(_lambda, std::any_cast<NthTypeOf<Indices>>(v[Indices])...);
      }
    }

    std::any invoke(std::vector<std::any> const& values) override
    {
      if constexpr (std::same_as<void, typename detail::lambda_traits<T>::result_t>) {
        invokeImpl(values, std::make_index_sequence<
                             std::tuple_size_v<typename detail::lambda_traits<T>::argument_t>>());
        return {};
      } else {
        return invokeImpl(values,
                          std::make_index_sequence<
                            std::tuple_size_v<typename detail::lambda_traits<T>::argument_t>>());
      }
    }

    T _lambda;
  };

public:
  /**
   * @brief Construct a new Meta Method object
   *
   * @tparam T type of the function pointer
   * @param function function pointer to be able to call
   * @param name name of the method
   * @param return_type return type name
   * @param arguments list of names of the arguments; needs to match the argument count of the
   * function pointer
   * @param isConst is it a const method
   * @param isVirtual is it a virtual method
   * @param isPureVirtual is it a purely virtual method
   * @param isOverride is the method overridden
   * @param isFinal is the method marked final
   */
  template <typename T>
  MetaMethod(T function, std::string_view name, std::string_view return_type,
             std::vector<std::string_view> arguments, uint8_t modes = Mode::NONE)
    : _p { std::make_unique<Impl_t<T>>(function) }
    , _name { name }
#ifdef MY_USE_REFLECTION
    , _returnType { detail::function_traits<T>::result_type_name }
#else
    , _returnType(return_type)
#endif
    , _arguments { arguments.begin(), arguments.end() }
    , _modes { static_cast<uint8_t>(
        modes | (detail::function_traits<T>::is_const::value ? Mode::CONST : Mode::NONE)
        | (detail::function_traits<T>::is_member::value ? Mode::MEMBER : Mode::NONE)) }
  {
#ifdef MY_USE_REFLECTION
    (void) return_type;
#else
#endif
    if (arguments.size() != std::tuple_size_v<typename detail::function_traits<T>::argument_t>) {
      throw std::runtime_error(std::format(
        "Argument count mismatch for meta method {}: named ({}) vs from function pointer ({})",
        _name, _arguments.size(),
        std::tuple_size_v<typename detail::function_traits<T>::argument_t>));
    }
  }

  /**
   * @brief Construct a new Meta Method from a lambda
   *
   * @tparam T the lambda type
   * @param lambda the lambda object
   * @param name name of the lambda
   * @param arguments the argument names of the lambda
   */
  template <typename T>
  MetaMethod(T&& lambda, std::string_view name, std::vector<std::string_view> arguments)
    : _p { std::make_unique<Impl_lambda_t<T>>(std::forward<T>(lambda)) }
    , _name { name }
#ifdef MY_USE_REFLECTION
    , _returnType { detail::lambda_traits<T>::result_type_name }
#endif
    , _arguments { arguments.begin(), arguments.end() }
    , _modes { static_cast<uint8_t>(
                 (detail::lambda_traits<T>::is_const::value ? Mode::CONST : Mode::NONE)
                 | (detail::lambda_traits<T>::is_member::value ? Mode::MEMBER : Mode::NONE))
               | Mode::LAMBDA }
  {
    if (arguments.size() != std::tuple_size_v<typename detail::lambda_traits<T>::argument_t>) {
      throw std::runtime_error(std::format(
        "Argument count mismatch for meta method {}: named ({}) vs from function pointer ({})",
        _name, _arguments.size(),
        std::tuple_size_v<typename detail::lambda_traits<T>::argument_t>));
    }
  }

  MetaMethod(MetaMethod const& other);
  MetaMethod(MetaMethod&& other);
  MetaMethod& operator=(MetaMethod const& other);
  MetaMethod& operator=(MetaMethod&& other);

  /**
   * @brief Invoke the method
   *
   * throws std::bad_any_cast if the argument types do not match exactly
   *
   * @param arguments arguments to invoke the method with
   * @return std::any
   */
  std::any invoke(std::vector<std::any> arguments);

  /**
   * @brief returns the name of the method
   *
   * @return std::string_view
   */
  std::string_view name() const;

  /**
   * @brief returns the name of the return type of the method
   *
   * @return std::string_view
   */
  std::string_view returnType() const;

  /**
   * @brief returns the description of the method
   *
   * @return std::string_view
   */
  std::string_view description() const;

  /**
   * @brief returns the list of argument names
   *
   * @return std::span<std::string const>
   */
  std::span<std::string const> argumentNames() const;

  /**
   * @brief queries constness
   *
   * @return true
   * @return false
   */
  bool isConst() const;

  /**
   * @brief queries virtual
   *
   * @return true
   * @return false
   */
  bool isVirtual() const;

  /**
   * @brief queries pure virtual
   *
   * @return true
   * @return false
   */
  bool isPureVirtual() const;

  /**
   * @brief queries if overridden
   *
   * @return true
   * @return false
   */
  bool isOverride() const;

  /**
   * @brief queries if final
   *
   * @return true
   * @return false
   */
  bool isFinal() const;

  /**
   * @brief queries whether it is a member function
   *
   * @return true
   * @return false
   */
  bool isMember() const;

  /**
   * @brief queries whether it represents a lambda
   *
   * @return true
   * @return false
   */
  bool isLambda() const;

private:
  std::unique_ptr<Impl> _p;
  std::string _name;
  std::string _returnType;
  std::vector<std::string> _arguments;
  //   std::vector<std::pair<std::string, std::string>> _arguments;
  std::string _description;
  uint8_t _modes;
};
} // namespace rtti
