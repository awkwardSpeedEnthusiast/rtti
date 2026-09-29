#pragma once

#include "rtti/metamethod.hpp"
#include "rtti/metaproperty.hpp"

#include <span>
#include <vector>

namespace rtti
{

/**
 * @brief RunTime Type Introspection for classes
 *
 * This class holds the description of a class for RTTI.
 */
class MetaClass
{
public:
  /**
   * @brief Construct a new Meta Class object
   *
   * @param name name of the class
   * @param methods the MetaMethods representing methods in this class
   * @param properties the MetaProperties representing properties in this class
   * @param bases the base classes of this class
   */
  MetaClass(std::string_view name, std::vector<MetaMethod> methods,
            std::vector<MetaProperty> properties, std::vector<MetaClass*> bases);
  MetaClass(MetaClass const&) = default;
  MetaClass(MetaClass&&) = default;
  MetaClass& operator=(MetaClass const&) = default;
  MetaClass& operator=(MetaClass&&) = default;
  auto operator<=>(MetaClass const& other) const = default;
  auto operator==(MetaClass const& other) const
  {
    return _name == other._name && _methods == other._methods && _properties == other._properties
        && _bases == other._bases;
  }

  /**
   * @brief return an iterable representation of a list in this class
   *
   * This method returns an iterable object (span) to one of the entity lists in MetaClass.
   *
   * \note currently only \a rtti::MetaMethod is supported
   *
   * @tparam T the type of list to get a span for
   * @return std::span<T>
   */
  template <typename T>
  std::span<T> range();

  /**
   * @brief return an iterable representation of a list in this class
   *
   * This method returns an iterable object (span) to one of the entity lists in MetaClass.
   *
   * \note currently only \a rtti::MetaMethod is supported
   *
   * @tparam T the type of list to get a span for
   * @return std::span<T>
   */
  template <typename T>
  std::span<T const> range() const;

  /**
   * @brief return the class name
   *
   * @return std::string_view
   */
  std::string_view classname() const;

private:
  std::vector<MetaMethod> _methods;
  std::vector<MetaProperty> _properties;
  std::vector<MetaClass*> _bases;
  std::string _name;
};
} // namespace rtti
