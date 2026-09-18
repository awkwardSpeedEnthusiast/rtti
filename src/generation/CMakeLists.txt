my_add_library(generation
  INTERFACE RELAXED_WARNINGS
  ALIAS Rtti::Generation
  HEADER
    include/rtti/method_extraction.hpp
    include/rtti/details/typetools.hpp
    include/rtti/details/argumentlist.hpp
    include/rtti/details/methodtrait.hpp
  INCLUDES
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
  DEPENDS
    INTERFACE Rtti::Structures
)
target_include_directories(generation INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(generation INTERFACE "-freflection")
