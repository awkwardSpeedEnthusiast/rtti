#pragma once

#include "rtti/metamethod.hpp"
#include <string_view>

namespace rtti
{

/**
 * @brief A Meta property
 *
 * A property collects methods which are connected accessing potentially a single data member.
 * This is a getter, a setter and some notification mechanism.
 */
class MetaProperty
{
public:
  /**
   * @brief Construct a new constant Meta Property object
   *
   * @param name name of the property
   * @param description a description
   * @param isTransient
   * @param getter the getter accessor to the property
   */
  MetaProperty(std::string_view name, std::string_view description, bool isTransient,
               MetaMethod getter);
  /**
   * @brief Construct a new read-only Meta Property object
   *
   * @param name name of the property
   * @param description a description
   * @param isTransient
   * @param getter the getter accessor to the property
   * @param notifier the notifier object
   */
  MetaProperty(std::string_view name, std::string_view description, bool isTransient,
               MetaMethod getter, MetaMethod notifier);
  /**
   * @brief Construct a new read/write Meta Property object
   *
   * @param name name of the property
   * @param description a description
   * @param isTransient
   * @param getter the getter accessor to the property
   * @param setter the setter accessor to the property
   * @param notifier the notifier object
   */
  MetaProperty(std::string_view name, std::string_view description, bool isTransient,
               MetaMethod getter, MetaMethod setter, MetaMethod notifier);
  MetaProperty(MetaProperty const& other) = default;
  MetaProperty(MetaProperty&& other) = default;
  MetaProperty& operator=(MetaProperty const& other) = default;
  MetaProperty& operator=(MetaProperty&& other) = default;
  ~MetaProperty() = default;
  auto operator<=>(MetaProperty const& other) const = default;
  auto operator==(MetaProperty const& other) const
  {
    return _name == other._name && _getter == other._getter && _setter == other._setter
        && _notifier == other._notifier;
  }

  /**
   * @brief return the name of the property
   *
   * @return std::string_view
   */
  std::string_view name() const;

  /**
   * @brief return the description of the property
   *
   * @return std::string_view
   */
  std::string_view description() const;

  /**
   * @brief return the getter method of the property
   *
   * @return MetaMethod
   */
  MetaMethod getter() const;

  /**
   * @brief return the setter method of the property if applicable
   *
   * @return std::optional<MetaMethod>
   */
  std::optional<MetaMethod> setter() const;

  /**
   * @brief return the notifier object of the property if applicable
   *
   * @return std::optional<MetaMethod>
   */
  std::optional<MetaMethod> notifier() const;

  /**
   * @brief returns whether the property is marked transient
   *
   * @return true
   * @return false
   */
  bool isTransient() const;

  /**
   * @brief query whether the property is read-only
   *
   * @return true
   * @return false
   */
  bool readOnly() const;

  /**
   * @brief query whether the property is constant
   *
   * @return true
   * @return false
   */
  bool constant() const;

  /**
   * @brief access the value of an object's property
   *
   * @param object the object to retrieve the property value from
   * @return std::any
   */
  std::any value(std::any object);

  /**
   * @brief try to set a value to the property of an object
   *
   * returns true if the setter could be called, false if not.
   *
   * @param object the object to set the property for
   * @param value the new value
   * @return true on success
   * @return false
   */
  bool setValue(std::any object, std::any value);

private:
  std::string _name;
  std::string _description;
  MetaMethod _getter;
  std::optional<MetaMethod> _setter;
  std::optional<MetaMethod> _notifier;
  bool _isTransient;
};
} // namespace rtti