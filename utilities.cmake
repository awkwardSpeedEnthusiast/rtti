include_guard(GLOBAL)

########################################################################################
# Declarative library and executable declaration
# 
# my_add_library
#    Define a library
#    Positional:
#      _NAME: name of the library
#    Flags:
#      INTERFACE: this is an interface library
#      STATIC: this is a static library
#      OBJECT: this is an object library
#      AUTORCC: CMake automatically compile Qt resource files
#      AUTOMOC: CMake automatically use Qt meta object compiler
#      AUTOUIC: CMake automatically use ui compiler
#    Single value arguments:
#      ALIAS: Alias target name
#    Multi value arguments:
#      HEADER: header files
#      SOURCE: source files
#      RESOURCES: Qt resource files
#      FORMS: Qt UI forms
#      QMLS: Qt QML files
#      DEPENDS: libraries or CMake targets to depend on
#      INCLUDES: include directories
#      DEFINES: compile definitioons
#      
# my_add_executable
#    Define an executable
#    Positional:
#      _NAME: name of the executable
#    Flags:
#      AUTORCC: CMake automatically compile Qt resource files
#      AUTOMOC: CMake automatically use Qt meta object compiler
#      AUTOUIC: CMake automatically use ui compiler
#      INSTALL: do install the target
#    Multi value arguments:
#      HEADER: header files
#      SOURCE: source files
#      RESOURCES: Qt resource files
#      FORMS: Qt UI forms
#      QMLS: Qt QML files
#      DEPENDS: libraries or CMake targets to depend on
#      INCLUDES: include directories
#      DEFINES: compile definitioons
#
########################################################################################

set(CMAKE_INCLUDE_CURRENT_DIR ON)  
include(GenerateExportHeader)
include(coverage)

macro(my_add_library)
set(_current_source_directory ${CMAKE_CURRENT_SOURCE_DIR})
set(_current_binary_directory ${CMAKE_CURRENT_BINARY_DIR})
_my_add_library(${ARGV})
endmacro()

macro(my_add_executable)
set(_current_source_directory ${CMAKE_CURRENT_SOURCE_DIR})
set(_current_binary_directory ${CMAKE_CURRENT_BINARY_DIR})
_my_add_executable(${ARGV})
endmacro()

macro(_add_properties_to_target _NAME)
  set(flags INTERFACE STATIC SHARED PLUGIN OBJECT NOCOVERAGE RELAXED_WARNINGS AUTORCC AUTOMOC AUTOUIC INSTALL RUN_SERIAL DISABLED)
  set(single ALIAS EXPORT_MACRO OUTPUT_DIR WORKING_DIRECTORY TIMEOUT)
  set(multi HEADER SOURCE RESOURCES FORMS DEPENDS INCLUDES DEFINES QMLS COMPILE_OPTIONS INSTALL_HEADER LABELS)
  cmake_parse_arguments(PARSE_ARGV 1 A "${flags}" "${single}" "${multi}")

  # Handle Qt resource (.qrc) files
  set(_qrc_resources "")
  if (A_RESOURCES)
    foreach(_qrc_file ${A_RESOURCES})
      qt6_add_resources(_qrc_output ${_qrc_file})
      list(APPEND _qrc_resources ${_qrc_output})
    endforeach()
  endif()
  
  # Handle QML files by creating a Qt resource
  set(_qml_resources "")
  if (A_QMLS)
    qt_add_resources(_qml_resources "${_NAME}_qml"
      PREFIX "/qml/${_NAME}"
      FILES ${A_QMLS}
    )
  endif()

  target_sources(${_NAME} PRIVATE ${_qrc_resources} ${_qml_resources})

  if(A_EXPORT_MACRO)
    generate_export_header(${_NAME} EXPORT_MACRO_NAME ${A_EXPORT_MACRO})
  endif()

  if (NOT A_RELAXED_WARNINGS)
    target_compile_options(${_NAME} PRIVATE
      $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
      $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
    )
  endif()

  if (A_DEPENDS)
    set(_interface_libs )
    set(_public_libs )
    set(_private_libs )
    set(_current "PUBLIC")
    foreach(item ${A_DEPENDS})
      if (${item} STREQUAL "PUBLIC")
        set(_current "PUBLIC")
      elseif (${item} STREQUAL "PRIVATE")
        set(_current "PRIVATE")
      elseif(${item} STREQUAL "INTERFACE")
        set(_current "INTERFACE")
      else()
        if (${_current} STREQUAL "PUBLIC")
          list(APPEND _public_libs ${item})
        elseif (${_current} STREQUAL "PRIVATE")
          list(APPEND _private_libs ${item})
        elseif (${_current} STREQUAL "INTERFACE")
          list(APPEND _interface_libs ${item})
        endif()
      endif()
    endforeach()
    target_link_libraries(${_NAME} INTERFACE "$<BUILD_INTERFACE:${_interface_libs}>")
    target_link_libraries(${_NAME} PUBLIC "$<BUILD_INTERFACE:${_public_libs}>")
    target_link_libraries(${_NAME} PRIVATE "$<BUILD_INTERFACE:${_private_libs}>")
  endif()

  if (A_INCLUDES)
    set(_interface_includes )
    set(_install_includes )
    set(_public_includes )
    set(_private_includes )
    set(_current "PUBLIC")
    foreach(item ${A_INCLUDES})
      if (${item} STREQUAL "PUBLIC")
        set(_current "PUBLIC")
      elseif (${item} STREQUAL "PRIVATE")
        set(_current "PRIVATE")
      elseif(${item} STREQUAL "INTERFACE")
        set(_current "INTERFACE")
      elseif(${item} STREQUAL "TO_INSTALL")
        set(_current "INSTALL")
      else()
        if (${_current} STREQUAL "PUBLIC")
          list(APPEND _public_includes ${item})
        elseif (${_current} STREQUAL "PRIVATE")
          list(APPEND _private_includes ${item})
        elseif (${_current} STREQUAL "INTERFACE")
          list(APPEND _interface_includes ${item})
        elseif (${_current} STREQUAL "INSTALL")
          list(APPEND _install_includes ${item})
        endif()
      endif()
    endforeach()
    target_include_directories(${_NAME} INTERFACE "$<BUILD_INTERFACE:${_interface_includes}>")
    target_include_directories(${_NAME} PUBLIC "$<BUILD_INTERFACE:${_public_includes}>")
    target_include_directories(${_NAME} PRIVATE "$<BUILD_INTERFACE:${_private_includes}>")
    target_include_directories(${_NAME} INTERFACE "$<INSTALL_INTERFACE:${_private_includes}>")
  endif()

  if (A_COMPILE_OPTIONS)
    target_compile_options(${_NAME} PUBLIC ${A_COMPILE_OPTIONS})
  endif()

  if (A_DEFINES)
    target_compile_definitions(${_NAME} ${A_DEFINES})
  endif()

  if (A_AUTORCC)
    set_target_properties(${_NAME} PROPERTIES AUTORCC ON)
  endif()
  if (A_AUTOMOC)
    set_target_properties(${_NAME} PROPERTIES AUTOMOC ON)
  endif()
  if (A_AUTOUIC)
    set_target_properties(${_NAME} PROPERTIES AUTOUIC ON)
  endif()

  if (A_INSTALL)
    install(TARGETS ${_NAME})
    if (A_INSTALL_HEADER)
      foreach(fi ${A_INSTALL_HEADER})
        cmake_path(SET _f ${fi})
        cmake_path(ABSOLUTE_PATH _f BASE_DIRECTORY ${_current_source_directory} OUTPUT_VARIABLE file)
        cmake_path(GET file PARENT_PATH _parent)
        cmake_path(RELATIVE_PART _parent BASE_DIRECTORY ${_current_source_directory} OUTPUT_VARIABLE dest)
        if ("${dest}" STREQUAL "")
          install(FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/include 
            PERMISSIONS 
              OWNER_READ OWNER_WRITE OWNER_EXECUTE 
              GROUP_READ GROUP_WRITE GROUP_EXECUTE
              WORLD_READ WORLD_WRITE WORLD_EXECUTE)
        else() 
          install(FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/${dest} 
            PERMISSIONS 
              OWNER_READ OWNER_WRITE OWNER_EXECUTE 
              GROUP_READ GROUP_WRITE GROUP_EXECUTE
              WORLD_READ WORLD_WRITE WORLD_EXECUTE)
        endif()
      endforeach()
    endif()
    install(EXPORT sdk DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake FILE ${_NAME}Config.cmake)
    if (A_EXPORT_MACRO)
      install(FILES ${_current_binary_directory}/${A_EXPORT_MACRO}_export.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
    endif()
  endif()

  if (A_OUTPUT_DIR)
    set_target_properties(${_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${A_OUTPUT_DIR})
  endif()

  add_coverage(${_NAME})
endmacro()

function(_my_add_library _NAME)
  set(flags INTERFACE STATIC SHARED PLUGIN OBJECT NOCOVERAGE RELAXED_WARNINGS AUTORCC AUTOMOC AUTOUIC INSTALL)
  set(single ALIAS EXPORT_MACRO OUTPUT_DIR)
  set(multi HEADER SOURCE RESOURCES FORMS DEPENDS INCLUDES DEFINES QMLS COMPILE_OPTIONS INSTALL_HEADER)
  cmake_parse_arguments(PARSE_ARGV 1 A "${flags}" "${single}" "${multi}")

  if(A_INTERFACE)
    message(STATUS "Adding interface library ${_NAME} (alias: ${A_ALIAS})")
    add_library(${_NAME} INTERFACE ${A_HEADER})
  elseif(A_OBJECT)
    message(STATUS "Adding object library ${_NAME} (alias: ${A_ALIAS})")
    add_library(${_NAME} OBJECT ${A_HEADER} ${A_SOURCE} ${A_RESOURCES} ${A_FORMS} ${QMLS})
  elseif(A_STATIC)
    message(STATUS "Adding static library ${_NAME} (alias: ${A_ALIAS})")
    add_library(${_NAME} STATIC ${A_HEADER} ${A_SOURCE} ${A_RESOURCES} ${A_FORMS} ${QMLS})
  elseif(A_SHARED OR A_PLUGIN)
    message(STATUS "Adding shared library ${_NAME} (alias: ${A_ALIAS})")
    add_library(${_NAME} SHARED ${A_HEADER} ${A_SOURCE} ${A_RESOURCES} ${A_FORMS} ${QMLS})
  else()
    message(FATAL_ERROR "No library type provided!")
  endif()

  _add_properties_to_target(${ARGV})

  if (A_ALIAS)
    add_library(${A_ALIAS} ALIAS ${_NAME})
  endif()
endfunction()

function(_my_add_executable _NAME)
  set(flags  AUTORCC AUTOMOC AUTOUIC INSTALL)
  set(single EXPORT_MACRO OUTPUT_DIR)
  set(multi HEADER SOURCE RESOURCES FORMS DEPENDS INCLUDES DEFINES QMLS COMPILE_OPTIONS INSTALL_HEADER)
  cmake_parse_arguments(PARSE_ARGV 1 A "${flags}" "${single}" "${multi}")

  message(STATUS "Adding executable ${_NAME}")
  add_executable(${_NAME} ${A_HEADER} ${A_SOURCE} ${A_RESOURCES} ${A_FORMS} ${A_QMLS})

  _add_properties_to_target(${ARGV})
endfunction()

