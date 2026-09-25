# RunTime Type Introspection

This repository is an experiment, not meant for productive use - at least for the
time being.

Since the reflection feature of C++26 is still fresh, I wanted to see, what can be
done with it and whether we can, at some point perhaps get rid of the meta object
compiler from Qt for the creation of RTTI classes, and yes, that's where the 
classes are inspired of.

## Build

As for building, this repo goes low profile:

* **cmake** meta build system
* **python3 + conan** for dependency management (even though it might be overkill)
* **gtest** for unit-testing (pulled/built by conan)

The rest is pure C++ 26 standard.

### Presets

This repository comes with some CMake presets. At the time of writing this document,
only gcc (version 16.2.1) had an implementation of reflections and `stl::meta`, so 
it is only tested with this compiler so far.

## Features and Usage

This code actually now has 7 public interface headers.
* `rtti/metamethod.hpp`
* `rtti/metaclass.hpp`
* `rtti/metaproperty.hpp`
* `rtti/method_extraction.hpp`
* `rtti/class_extraction.hpp`
* `rtti/property_definition.hpp`
* `rtti/property_extraction.hpp`

### MetaMethod

The class `MetaMethod` became a bit fancier than originally planned, not only 
holding a description, but being able to invoke the method/function it represents.
Unfortunately it comes with the casting restrictions of `std::any`.

This class uses reflection only if enabled, so it should compile with clang/msvc as 
well.

### MetaProperty

Often you have one data member in your class, but obviously you don't want to expose the
member itself, but provide accessors (getter + setter) and optimally a notifier (like Qt
signal or `boost::signals2::signal`). So these 3 methods are connected. The class 
`MetaProperty` reflects this connection collecting up to 3 `MetaMethod`s.

To identify them automatically, some gymnastics need to be made with annotations. There is
a description in `rtti/property_definition.hpp` and some example in the [tests](tests/generation/data/classeswithannotations.hpp).

### MetaClass

Hosts a list of methods.
And other properties - hopefully.

This class doesn't use reflections at all, should compile with clang/msvc as well.

### createMethod

Creation method for MetaMethods using reflections and `std::meta`. And some 
template magic.

### createProperty

Creation method for MetaPropertys using reflections and `std::meta`. And some serious
template and annotation magic. 

### createClass

Creation method for MetaClass using reflections and `std::meta`. And more template
magic.

## Open points

Pushing the code today won't stop me from adding more stuff.
Some of the things I'm thinking about adding:

* **annotations**: yet an other C++26 feature. Enriching the code with annotations could 
  * add descriptions for classes, methods, etc you could display to users
  * add meta-data identifying
    * Qt-like signals
    * notifier (callback) subscriptions, like `boost::signals2`
* **namespaces**: they are currently ignored 
* **static member functions**: MetaMethods support them already, MetaClasses don't
* **base classes**: MetaClass should host a list of base classes
* enable **type conversion** for the MetaMethod invoke method for convertable types
* **template functions** in MetaMethods; currently you have to instantiate to use it 
* C++20 module: compilation times become serious with all that compile-time computation reflections require
* ...

