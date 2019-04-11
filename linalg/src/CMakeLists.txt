### >> Create core target, so we may link modules to it directly <<
### >> This allow to propagate dependencies upwards sanely
pybind11_add_module(core MODULE THIN_LTO core.cc)

# MSVC does not include <cmath> constants, unless _USE_MATH_DEFINES is defined.
target_compile_definitions(core
  PUBLIC
    "$<$<CXX_COMPILER_ID:MSVC>:_USE_MATH_DEFINES>"
  )

# Set the exception handling model and increase the number of addressable sections
target_compile_options(core
  PUBLIC
    "$<$<CXX_COMPILER_ID:MSVC>:/EHsc>"
  PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/bigobj>"
  )

# Let clang-cl behave like MSVC when it comes to the preprocessor
target_compile_options(core
  PUBLIC
    "$<$<AND:$<C_COMPILER_ID:Clang>,$<BOOL:${WIN32}>>:-Xclang;-fms-compatibility>"
    "$<$<AND:$<CXX_COMPILER_ID:Clang>,$<BOOL:${WIN32}>>:-Xclang;-fms-compatibility>"
  )

target_sources(core
  PRIVATE
    export_linalg.cc
    dimension.cc
  )

set_target_properties(core
  PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    POSITION_INDEPENDENT_CODE 1
    VISIBILITY_INLINES_HIDDEN 1
  )

# xtensor-blas and xtensor-io are not proper targets
target_include_directories(core
  PRIVATE
    ${xtensor-blas_INCLUDE_DIRS}
    ${xtensor-io_INCLUDE_DIRS}
  )

target_link_libraries(core
  PRIVATE
    IntelMKL
    xtensor
    xtensor-python
    NumPy
    "$<$<BOOL:${MSVC}>:Ws2_32>"
  )

install(
  TARGETS
    core
  ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/linalg
  RUNTIME
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/linalg
  )
