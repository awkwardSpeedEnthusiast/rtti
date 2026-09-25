#pragma once
#include "property_definition.hpp"
#include "rtti/details/methodlist.hpp"
#include "rtti/details/propertystruct.hpp"
#include "rtti/details/tuplebuilder.hpp"
#include "rtti/metamethod.hpp"
#include "rtti/metaproperty.hpp"
#include "rtti/method_extraction.hpp"
#include "rtti/property_definition.hpp"

#include <meta>
#include <type_traits>

namespace rtti
{

template <std::meta::info i>
consteval auto name_member()
{
  template for (constexpr auto m : std::define_static_array(
                  std::meta::members_of(i, std::meta::access_context::unchecked().via(i))))
  {
    if constexpr (std::meta::is_variable(m)
                  && std::meta::type_of(m) == details::property_struct_getting::enum_type<i>()) {
      return m;
    }
  }
  return ^^void;
}

template <std::meta::info clazz>
struct extract {
  using T = [:clazz:];
  using ps = property_struct<T>;
  using P = ps::P;
  using E = ps::E;
  static constexpr auto name_mem = ps::name_d;
  static constexpr auto type_mem = ps::type_d;
  template <std::meta::info a, E n>
  static consteval auto defines_property()
  {
    return std::meta::extract<P>(a).[:name_mem:] == n;
  }

  template <std::meta::info f, E n>
  consteval static auto method_defines_property()
  {
    template for (constexpr auto a :
                  std::define_static_array(std::meta::annotations_of_with_type(f, ^^P)))
    {
      if constexpr (defines_property<a, n>()) {
        return true;
      }
    }
    return false;
  }

  template <std::meta::info f, E n>
  consteval static auto property_of()
  {
    template for (constexpr auto a :
                  std::define_static_array(std::meta::annotations_of_with_type(f, ^^P)))
    {
      if constexpr (defines_property<a, n>()) {
        return std::meta::extract<P>(a);
      }
    }
  }
};

/**
 * @brief Find a method with specific annotations
 *
 * For a property \a n, a method is searched, which is annotated with the property struct
 * and marked with type \a which.
 *
 * @tparam clazz the class to search for the method
 * @tparam E the property name list enum
 * @tparam n the property enum value of interest
 * @tparam which the type of method to look for
 * @return consteval
 */
template <std::meta::info clazz, typename E, E n, rtti::property::types which>
consteval auto find_method()
{
  template for (constexpr auto member : std::define_static_array(
                  std::meta::members_of(clazz, std::meta::access_context::unchecked().via(clazz))))
  {
    if constexpr (rtti::details::isFunction<member>()) {
      if constexpr (extract<clazz>::template method_defines_property<member, n>()) {
        constexpr auto def = extract<clazz>::template property_of<member, n>();
        if constexpr (def.[:extract<clazz>::type_mem:]
                      == which
                      && ((which == rtti::property::types::getter && std::meta::is_const(member))
                          || (which != rtti::property::types::getter
                              && !std::meta::is_const(member)))) {
          return member;
        }
      }
    }
  }
  return ^^void;
}

/**
 * @brief define a single property from the class's property description
 *
 * This struct keeps the information about a single property defined by the property struct
 * in the hosting class.
 * Entrance point is the NT-template parameter, which needs to be the reflection of an enum
 * value inside the property struct.
 *
 * It contains reflections to getter, setter and notifier, if found
 *
 * Still needs to be done: check, that the getter returns the same type as tyoe of the
 * setter's single argument, as well as the restrictions on the notifier.
 *
 * @tparam e reflection of the enum value inside the enum of the property struct of the class
 */
template <std::meta::info e>
struct single_prop_t {
  static constexpr auto val = [:e:];
  static constexpr auto name = std::meta::identifier_of(e);
  static constexpr auto parent_enum = std::meta::parent_of(e);
  static constexpr auto property_struct = std::meta::parent_of(parent_enum);
  static constexpr auto clazz = std::meta::parent_of(property_struct);
  using T = [:clazz:];
  using E = [:parent_enum:];
  using P = [:property_struct:];

  static constexpr auto getter = find_method<clazz, E, val, rtti::property::types::getter>();
  static constexpr auto setter = find_method<clazz, E, val, rtti::property::types::setter>();
  static constexpr auto notifier = find_method<clazz, E, val, rtti::property::types::notifier>();
  static constexpr auto has_getter = !std::meta::is_type(getter) && std::meta::is_const(getter);
  static constexpr auto has_setter = !std::meta::is_type(setter);
  static constexpr auto has_notifier = !std::meta::is_type(notifier);

  static constexpr auto is_valid = has_getter;
  static consteval auto parse()
  {
    return std::define_static_array(
             std::meta::annotations_of_with_type(e, ^^rtti::property::Transient))
             .size()
         > 0;
  }
  static constexpr auto is_transient = parse();
};

/**
 * @brief extract the list of possible properties from the property name enum
 *
 * @tparam enum_r reflection of the enum type
 */
template <std::meta::info enum_r>
struct properties {
  static constexpr auto list = std::define_static_array(std::meta::enumerators_of(enum_r));
};

/**
 * @brief predicate to only list valid properties
 *
 * @tparam enum_value
 */
template <std::meta::info enum_value>
struct is_valid_property {
  static constexpr auto value = single_prop_t<enum_value>::is_valid;
};

/**
 * @brief Property list structure
 *
 * This struct is defined twice; this is the version for classes, which do not have a
 * property defining struct
 *
 * @tparam T the type of interest
 * @tparam v false
 */
template <typename T, bool v>
struct props_list : std::false_type {
  using property_tuple = std::tuple<>;
};

/**
 * @brief Property list structure
 *
 * Second part of the stuct for classes which do have a property defining struct.
 * The actual list is the list of types in the tuple.
 *
 * @tparam T the type of interest
 */
template <typename T>
struct props_list<T, true> : std::true_type {
  static constexpr auto enum_r = property_struct<T>::name_e;
  using property_tuple =
    tuple_builder_t<enum_r, properties, is_valid_property, single_prop_t>::tuple_type;
};

/**
 * @brief Alias including the check whether the property struct exists
 *
 * @tparam T the class of interest
 */
template <typename T>
using property_list = props_list<T, property_struct<T>::value>;

/**
 * @brief Create a MetaProperty object
 *
 * @tparam trait the trait to create from
 * @return rtti::MetaProperty
 */
template <typename trait>
rtti::MetaProperty createProperty()
{
  static rtti::MetaProperty property = []() {
    rtti::MetaMethod getter = createMethod<trait::getter>();
    if constexpr (trait::has_notifier && trait::has_setter) {
      rtti::MetaMethod setter = createMethod<trait::setter>();
      rtti::MetaMethod notifier = createMethod<trait::notifier>();
      return rtti::MetaProperty { trait::name, trait::name, trait::is_transient,
                                  getter,      setter,      notifier };
    } else if constexpr (trait::has_notifier) {
      rtti::MetaMethod notifier = createMethod<trait::notifier>();
      return rtti::MetaProperty { trait::name, trait::name, trait::is_transient, getter, notifier };
    } else {
      return rtti::MetaProperty { trait::name, trait::name, trait::is_transient, getter };
    }
  }();
  return property;
}

/**
 * @brief convert the tuple types to an array of MetaProperty
 *
 * @tparam tuple_t
 * @return constexpr auto
 */
template <typename tuple_t>
constexpr auto get_properties_from_tuple()
{
  if constexpr (std::tuple_size_v<tuple_t> == 0) {
    return std::array<rtti::MetaProperty, 0> {};
  } else {
    constexpr auto convert = []<typename T>(T&&) { return createProperty<T>(); };
    constexpr auto get_array = [convert]<typename... Args>(auto&&... x) {
      return std::array { convert(std::forward<decltype(x)>(x))... };
    };
    return std::apply(get_array, tuple_t {});
  }
}

} // namespace rtti
