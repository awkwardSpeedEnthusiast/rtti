my_add_library(generation
  INTERFACE RELAXED_WARNINGS
  ALIAS Rtti::Generation
  HEADER
    include/rtti/class_extraction.hpp
    include/rtti/property_definition.hpp
    include/rtti/property_extraction.hpp
    include/rtti/details/classtrait.hpp
    include/rtti/details/propertystruct.hpp
    include/rtti/details/tuplebuilder.hpp
  INCLUDES
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    INTERFACE Rtti::Structures
  COMPILE_OPTIONS 
    INTERFACE -freflection
)
target_include_directories(generation INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(generation INTERFACE "-freflection")
