#pragma once

#include "rtti/details/metatools.hpp"

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
  using raw_args_t = std::tuple<Args...>;
  using argument_t = std::tuple<Args...>;
  using is_member = std::false_type;
  using class_t = void*;
  using is_const = std::false_type;
#ifdef MY_USE_REFLECTION
  static constexpr auto result_type_name = type_name<R>();
#endif
};

template <typename R, typename Cls, typename... Args>
struct function_traits<R (Cls::*)(Args...) const> {
  using signature_t = R (Cls::*)(Args...);
  using result_t = R;
  using raw_args_t = std::tuple<Args...>;
  using argument_t = std::tuple<Cls*, Args...>;
  using class_t = Cls*;
  using is_member = std::true_type;
  using is_const = std::true_type;
#ifdef MY_USE_REFLECTION
  static constexpr auto result_type_name = type_name<R>();
#endif
};

template <typename R, typename Cls, typename... Args>
struct function_traits<R (Cls::*)(Args...)> {
  using signature_t = R (Cls::*)(Args...);
  using result_t = R;
  using raw_args_t = std::tuple<Args...>;
  using argument_t = std::tuple<Cls*, Args...>;
  using class_t = Cls*;
  using is_member = std::true_type;
  using is_const = std::false_type;
#ifdef MY_USE_REFLECTION
  static constexpr auto result_type_name = type_name<R>();
#endif
};

template <typename T>
struct lambda_traits {
  using op_tr = function_traits<decltype(&std::remove_reference_t<T>::operator())>;
  using signature_t = op_tr::signature_t;
  using result_t = op_tr::result_t;
  using argument_t = op_tr::raw_args_t;
  using class_t = op_tr::class_t;
  using is_member = op_tr::is_member;
  using is_const = op_tr::is_const;
#ifdef MY_USE_REFLECTION
  static constexpr auto result_type_name = op_tr::result_type_name;
#endif
};

} // namespace rtti::detail
