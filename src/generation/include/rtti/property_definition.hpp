#pragma once

namespace rtti::property
{
/**
 * @brief Property definition
 *
 * Properties are compound entities of a class.
 * It encapsulates one value of some type - not necessarily a basic type.
 *
 * It provides at least a way to read the value (getter).
 * Read/write properties offer a way to change the value (setter).
 * Ideally a property should provide a way to notify observers of changes (notifier)
 *
 * We could enrich the property as well with other information like
 *   * Default value
 *   * Whether it is a transient value or should be persisted
 *
 * That said a Property comes with the following:
 *   * Name
 *   * Type
 *   * Getter
 *   * [Setter]
 *   * [Notifier]
 *   * [Default value]
 *   * [Transient-flag]
 *
 * To make this work with C++26 annotations, the class needs to define the properties it wants to
 * expose:
 *   struct Properties {
 *     enum class names {
 *       property1 [[=Transient]],
 *       property2,
 *     };
 *     names name;
 *     rtti::properties::types type;
 *   };
 *
 * Flags need to be attached as annotations to the property name enum.
 * This Properties struct will be used in the annotations attached to the methods acting as
 * getter/setter/notifier.
 * If you want to use a default value, a member (static or non-static) with the respective value
 * needs to be annotated as follows:
 *
 * int _value [[=MyClass::Properties{MyClass::Properties::names::property1,
 *                                   rtti::property::types::member}]]
 *            [[=rtti::property::Default]] = 42;
 *
 * A tagging of a function needs to fullfil the following constraints (if tagged anyway, we ignore
 * it):
 *    * getter:
 *        * method must be const
 *        * method must not be static
 *        * method must not return void
 *    * setter:
 *        * method must not be const
 *        * method must have exactly one argument
 *    * notifier:
 *        * if tagged with Qt_Signal (actual signal in Qt framework)
 *            * any number of arguments are allowed
 *            * only void as return type is allowed
 *        * else (represents more a subscription function)
 *            * method must not return void (something like boost::signals2::connection would be
 *              appropriate)
 *            * method must have exactly one argument of a callable type (callback)
 *
 * A single property definition of a class will be rejected if:
 *    * number of found getters is other than one
 *    * if more than one setter is tagged and the type of the argument doesn't match the
 *      return type of the getter
 *    * if more than one notifier is tagged
 *
 * We find the properties defining struct by the following:
 *    * it is a private inner struct
 *    * it contains exactly one non-static data member of type rtt::property::type.
 *    * it contains one enum [class] (containing the names of the properties) and exactly one
 *      non-static data member of that type
 *    * it doesn't contain anything else
 */

/**
 * @brief specifier to tag methods/member fields for properties
 *
 */
enum class types {
  getter,   // tag method as getter; non-void const method without arguments only
  setter,   // tag method as setter; void non-const method with exactly one argument only
  notifier, // tag method as notifier; method only (see restrictions further up)
  member,   // tag data member to belong to property; data only
};

/**
 * @brief Tag on property enum name, do not persist
 */
struct Transient {
};

/**
 * @brief Tag on property member to identify defaults
 */
struct Default {
};

} // namespace rtti::property
