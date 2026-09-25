my_add_library(structures
  STATIC
  ALIAS Rtti::Structures
  HEADER
    include/rtti/metamethod.hpp
    include/rtti/details/functiontraits.hpp
    include/rtti/metaproperty.hpp
    include/rtti/metaclass.hpp
  SOURCE
    src/metamethod.cpp
    src/metaclass.cpp
    src/metaproperty.cpp
  INCLUDES
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
