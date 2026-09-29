my_add_library(structures
  STATIC
  ALIAS Rtti::Structures
  HEADER
    include/rtti/metamethod.hpp
    include/rtti/details/methodtools.hpp
    include/rtti/metaproperty.hpp
    include/rtti/metaclass.hpp
  SOURCE
    src/metaclass.cpp
    src/metaproperty.cpp
  INCLUDES
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(structures PUBLIC "-freflection")
