
#pragma once

#include <cstddef>
#include <meta>
#include <tuple>

/**
 * @brief Generic helper objects to create tuples from reflection lists
 *
 * After creating the third tuple builder I decided to have a generic one.
 * This tuple builder takes as template parameters:
 *   * the reflection of the object containing the list of interest (method for argument-list
 *       class for method-list, etc)
 *   * \a Func strategy: this NTT struct has a static constexpr member \a list, which is
 *       the list of reflections to be converted.
 *   * \a Pred strategy: this NTT struct is a bool predicate, it contains a member \a value
 *       which is true if the respective list entry shall be accepted. This is helpful to filter
 *       nested classes off the method list
 *   * \a Transform: this NTT struct is the trait to be constructed for the tuple (eg argument_t
 *       for argument lists, method_t for method lists, etc.)
 *
 * Please note, that \a Func takes the provided \a info as template parameter, while \a Pred and
 * \a Transform will get the list elements reflection as template parameter.
 */
namespace rtti::details
{
template <std::meta::info info, template <std::meta::info b> typename Transform, bool b>
struct ttt : std::false_type {
  using type = void;
};

template <std::meta::info info, template <std::meta::info b> typename Transform>
struct ttt<info, Transform, true> : std::true_type {
  using type = Transform<info>;
};

// collect types depending on pred
// the detour via ttt is necessary since we do not want to generate the type if it would be
// rejected (would be pointless in best case or lead to compiler errors if it cannot be
// constructed at all).
template <std::meta::info info, template <std::meta::info b> typename Func,
          template <std::meta::info c> typename Pred,
          template <std::meta::info d> typename Transform, size_t I, typename... Args>
struct tuple_builder_helper {
  static constexpr auto list = Func<info>::list;
  static constexpr auto instance = list[I - 1];
  static constexpr auto condition = Pred<instance>::value;
  using newType = ttt<instance, Transform, condition>::type;
  using vt = tuple_builder_helper<info, Func, Pred, Transform, I - 1, newType, Args...>::tuple_type;
  using vf = tuple_builder_helper<info, Func, Pred, Transform, I - 1, Args...>::tuple_type;
  using tuple_type = std::conditional_t<condition, vt, vf>;
};
// break recursion: no elements found
template <std::meta::info m, template <std::meta::info b> typename Func,
          template <std::meta::info c> typename Pred,
          template <std::meta::info d> typename Transform>
struct tuple_builder_helper<m, Func, Pred, Transform, 0U> {
  using tuple_type = std::tuple<>;
};
// break recursion: convert elements to tuple
template <std::meta::info m, template <std::meta::info b> typename Func,
          template <std::meta::info c> typename Pred,
          template <std::meta::info d> typename Transform, typename... Args>
struct tuple_builder_helper<m, Func, Pred, Transform, 0U, Args...> {
  using tuple_type = std::tuple<Args...>;
};
} // namespace rtti::details

namespace rtti
{
/**
 * @brief Create a tuple of trait types from a reflection list
 *
 * @tparam m the reflection containing the list
 * @tparam Func getter for the list from \a m
 * @tparam Pred conditional to accept or reject an element of the list
 * @tparam Transform trait struct to be used as type for the tuple element
 */
template <std::meta::info m, template <std::meta::info b> typename Func,
          template <std::meta::info b> typename Pred,
          template <std::meta::info b> typename Transform>
struct tuple_builder_t {
  static constexpr auto size = Func<m>::list.size();
  using tuple_type =
    rtti::details::tuple_builder_helper<m, Func, Pred, Transform, size>::tuple_type;
};

} // namespace rtti
