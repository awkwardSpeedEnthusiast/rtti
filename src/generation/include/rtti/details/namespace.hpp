#pragma once

#include <meta>

/**
 * @brief Namespace discovery
 *
 * In this file we have functions retrieving namespace names for a provided free entity. This entity
 * could be a free function, a class/struct or a namespace itself.
 *
 * The algorithm used here might not be optimal, but it was the first one I came up with and
 * namespaces usually are not stacked too deeply, so let's not do premature optimization - yet.
 */
namespace rtti
{
namespace details
{

/**
 * @brief Returns the number of parents/namespaces some entity has
 *
 * @tparam NS the entity
 * @return consteval
 */
template <auto NS>
consteval auto nNamesp()
{
  if constexpr (std::meta::is_type(NS)) {
    if constexpr (std::meta::has_parent(NS)) {
      return nNamesp<std::meta::parent_of(NS)>();
    } else {
      return 0;
    }
  } else if constexpr (std::meta::has_parent(NS)) {
    return nNamesp<std::meta::parent_of(NS)>() + 1;
  } else {
    return 0;
  }
}

/**
 * @brief Get the name of a namespace
 *
 * Calculate a namespace/parent name of an object.
 * This function is to be called recursively until the @a Is counter is decreased to zero.
 * It assumes, that the @a Is is smaller than the number of parents.
 *
 * @tparam T The reflection to get the parent name for,
 * @tparam Is how much parents to we have to go up still,
 * @return consteval
 */
template <std::meta::info T, size_t Is>
consteval auto get_namespace()
{
  if constexpr (Is == 0) {
    return std::meta::identifier_of(T);
  } else {
    return get_namespace<std::meta::parent_of(T), Is - 1>();
  }
}

/**
 * @brief calculate an array of namespace/parent names for an entity
 *
 * @tparam T the reflection of the entity to create the list for
 * @tparam Is the number of stacked namespaces the entity is in
 * @return consteval
 */
template <std::meta::info T, size_t... Is>
consteval auto build_namespace_array_impl(std::index_sequence<Is...>)
{
  std::array<std::string_view, sizeof...(Is)> names {};

  template for (constexpr size_t Idx : { Is... })
  {
    names[Idx] = get_namespace<T, Idx + 1>();
  }

  return names;
}
} // namespace details

/**
 * @brief Retrieve an arry with the names of the namespaces an entity resides in
 *
 * @tparam T the reflection of the object to retrieve the namespace list for
 * @return consteval
 */
template <std::meta::info T>
consteval auto get_namespace_array()
{
  constexpr size_t ns_count = details::nNamesp<T>();
  return details::build_namespace_array_impl<T>(std::make_index_sequence<ns_count> {});
}
} // namespace rtti
