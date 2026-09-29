#pragma once

#include "rtti/metaclass.hpp"
#include "rtti/property_extraction.hpp"

#include <meta>
#include <type_traits>

namespace rtti
{
template <typename T>
rtti::MetaClass* createClass();
}

namespace rtti::details
{
/**
 * @brief Type erasure: functor to create a base-class
 *
 */
using BaseCreator = rtti::MetaClass* (*) ();

/**
 * @brief POD representing a base-class
 * (contains a type-erased creator)
 */
struct base_t {
  BaseCreator creator;
};

/**
 * @brief Concrete creator function to create a MetaClass to be used as base-class
 *
 * @tparam clazz the class reflection of the base class
 * @return rtti::MetaClass*
 */
template <std::meta::info clazz>
rtti::MetaClass* baseCreator_i()
{
  return rtti::createClass<typename[:std::meta::type_of(clazz):]>();
}

/**
 * @brief retrieve a list of base-class info structs for a class
 *
 * This extracts a list of base-class-info/init structs from a class using C++26 for loops
 * which are much more performant than the recursively generated tuple.
 *
 * Then the array is filled with a type erased creator function.
 *
 * \todo Checkout wheter it is possible to use range-filters/algorithms here.
 *
 * @tparam T
 * @return consteval
 */
template <std::meta::info clazz>
  requires(std::meta::is_class_type(clazz))
consteval auto get_base_list()
{
  std::array<base_t, std::define_static_array(
                       std::meta::bases_of(clazz, std::meta::access_context::unprivileged()))
                       .size()>
    result {};
  std::size_t idx [[maybe_unused]] = 0;
  template for (constexpr auto a : std::define_static_array(
                  std::meta::bases_of(clazz, std::meta::access_context::unprivileged())))
  {
    result[idx++].creator = baseCreator_i<a>;
  }
  return result;
}

/**
 * @brief retrieve a list of method info structs for a class
 *
 * This extracts a list of method-info/init structs from a class using C++26 for loops
 * which are much more performant than the recursively generated tuple.
 * At first it calculates the number of elements; this is necessary for the std::array
 * return value (std::vector is not possible due to its use of new).
 *
 * Then the array is filled with the consteval creatable init-struct for MetaMethod.
 *
 * \todo Checkout wheter it is possible to use range-filters/algorithms here.
 *
 * @tparam T
 * @return consteval
 */
template <typename T>
consteval auto get_method_infos()
{
  constexpr std::size_t count = []() {
    std::size_t c = 0;
    template for (constexpr auto m : std::define_static_array(
                    std::meta::members_of(^^T, std::meta::access_context::unprivileged())))
    {
      if constexpr (rtti::details::isFunction<m>()) {
        c++;
      }
    }
    return c;
  }();

  std::array<rtti::details::f_info, count> result {};
  std::size_t idx = 0;

  template for (constexpr auto m : std::define_static_array(
                  std::meta::members_of(^^T, std::meta::access_context::unprivileged())))
  {
    if constexpr (rtti::details::isFunction<m>()) {
      result[idx++] = createFromType<m, T>();
    }
  }

  return result;
}

/**
 * @brief extract the class name from a reflection
 *
 * This is necessary in particular for stl classes, which do not have an identifier.
 *
 * @tparam clazz the class to extract the name from
 * @return consteval
 */
template <std::meta::info clazz>
consteval auto classname()
{
  constexpr auto c = std::meta::dealias(clazz);
  if constexpr (std::meta::has_identifier(c)) {
    return std::meta::identifier_of(c);
  } else {
    return std::meta::display_string_of(c);
  }
}
/**
 * @brief basic trait defining properties of a class
 *
 * @tparam T type of the class
 */
template <typename T>
struct class_traits {
  using type_t = std::remove_pointer_t<std::remove_reference_t<std::remove_cv_t<T>>>;

  static constexpr auto const type_name = classname<^^T>();
  using property_tuple = typename property_list<T>::property_tuple;
};
} // namespace rtti::details
