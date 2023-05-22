cmake_minimum_required(VERSION 3.0)
project(uff VERSION 0.0.0 LANGUAGES CXX)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif()

# options - default is header only
option(UFF_BUILD_EXAMPLES "build examples" OFF)
option(UFF_BUILD_C_BINDING "build c binding" OFF)
option(UFF_BUILD_FORTRAN_MODULE "build fortran module" OFF)

# some build examples requires c binding
if (UFF_BUILD_EXAMPLES)
  set(UFF_BUILD_C_BINDING ON)
  set(UFF_BUILD_FORTRAN_MODULE ON)
endif()

# config
set(exe_dest "bin")
set(incl_dest "include")
set(lib_dest "lib")
set(cfg_dest "${lib_dest}/cmake/${PROJECT_NAME}")
set(gen_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(version_config "${gen_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${gen_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# sources
set(CXX_SRCS src/uff.cpp)
set(CXX_HEADERS src/uff.hpp)

# add library - either compiled or header only
if (UFF_BUILD_C_BINDING)
  add_library(uff ${CXX_SRCS})
  target_include_directories(uff
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${incl_dest}>)
  target_compile_options(uff PRIVATE -O3)
else()
  add_library(uff INTERFACE)
  target_include_directories(uff
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${incl_dest}>)
endif()

add_library(uff::uff ALIAS uff)
set_target_properties(uff PROPERTIES CXX_STANDARD 11)

if(UFF_BUILD_FORTRAN_MODULE)
  message("*** building additional fortran module ***")
  add_subdirectory(src/fmod)
endif(UFF_BUILD_FORTRAN_MODULE)

if(UFF_BUILD_EXAMPLES)
  message("*** building examples ***")
  add_subdirectory(examples)
endif(UFF_BUILD_EXAMPLES)

# configure config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
    "cmake/config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${cfg_dest}"
)

install(
    TARGETS uff
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION ${lib_dest}
    ARCHIVE DESTINATION ${lib_dest}
    INCLUDES DESTINATION "${incl_dest}"
)
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${cfg_dest}"
)

install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${namespace}"
    DESTINATION "${cfg_dest}"
)
install(
    FILES ${CXX_HEADERS}
    DESTINATION ${incl_dest}
)
