cmake_minimum_required(VERSION 3.0)
project(uffexamples VERSION 0.0.0 LANGUAGES Fortran)

if (NOT UFF_BUILD_EXAMPLES)
  find_package(uff REQUIRED)
endif()

add_executable(uffomg uffomg.F90)
target_link_libraries(uffomg PRIVATE uff::uff)

add_executable(uffomg_usemod uffomg_usemod.F90)
target_link_libraries(uffomg_usemod PRIVATE uff::ufffortran)
