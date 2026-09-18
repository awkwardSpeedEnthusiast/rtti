my_add_library(structures
  STATIC
  ALIAS Rtti::Structures
  HEADER
    include/rtti/metamethod.hpp
    include/rtti/details/functiontraits.hpp
  SOURCE
    src/metamethod.cpp
  INCLUDES
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
