#pragma once

#include <tuple>
#include <type_traits>

namespace rtti::detail
{
template <typename... T>
struct function_traits {
};

template <typename R, typename... Args>
struct function_traits<R (*)(Args...)> {
  using signature_t = R (*)(Args...);
  using result_t = R;
  using argument_t = std::tuple<Args...>;
  using is_member = std::false_type;
  using class_t = void*;
  using is_const = std::false_type;
};

template <typename R, typename Cls, typename... Args>
struct function_traits<R (Cls::*)(Args...) const> {
  using signature_t = R (Cls::*)(Args...);
  using result_t = R;
  using argument_t = std::tuple<Cls*, Args...>;
  using class_t = Cls*;
  using is_member = std::true_type;
  using is_const = std::true_type;
};

template <typename R, typename Cls, typename... Args>
struct function_traits<R (Cls::*)(Args...)> {
  using signature_t = R (Cls::*)(Args...);
  using result_t = R;
  using argument_t = std::tuple<Cls*, Args...>;
  using class_t = Cls*;
  using is_member = std::true_type;
  using is_const = std::false_type;
};
} // namespace rtti::detail
