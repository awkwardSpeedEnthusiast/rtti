my_add_test(structuretests 
  SOURCE 
    methodtests.cpp 
  DEPENDS 
    Rtti::Structures
  COMPILE_OPTIONS 
    PRIVATE -freflection
)
