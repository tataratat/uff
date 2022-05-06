enable_language(Fortran)

set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/UFFMODULES)

set(F_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/uff.F90)
add_library(ufffortran ${F_SRCS})
add_library(uff::ufffortran ALIAS ufffortran)
target_link_libraries(ufffortran PUBLIC uff)
target_include_directories(ufffortran
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_Fortran_MODULE_DIRECTORY}>)
install(
    TARGETS ufffortran
    EXPORT ${TARGETS_EXPORT_NAME}
    LIBRARY DESTINATION ${lib_dest}
    ARCHIVE DESTINATION ${lib_dest}
    INCLUDES DESTINATION ${incl_dest}
)

file(GLOB_RECURSE uffmods "${CMAKE_Fortran_MODULE_DIRECTORY}/*.mod")
install(
    FILES ${uffmods}
    DESTINATION ${incl_dest}
)
