cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(linalg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)

set(Python_ADDITIONAL_VERSIONS 3.7 3.6)  # adjust with CMake minimum FindPythonInterp
find_package(PythonLibsNew 3.6 REQUIRED)

set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage)
add_subdirectory(external/upstream)

include(ExternalProject)
ExternalProject_Add(linalg-core
   DEPENDS
     pybind11_external
     xsimd_external
     xtl_external
     xtensor_external
     xtensor-blas_external
     xtensor-io_external
     xtensor-python_external
   SOURCE_DIR
     ${CMAKE_CURRENT_SOURCE_DIR}/linalg
   CMAKE_ARGS
     -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
     -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
     -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
     -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
     -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
     -DCMAKE_CXX_STANDARD_REQUIRED=ON
     -DCMAKE_CXX_EXTENSIONS=OFF
     -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}
     -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR}
     -DPYTHON_LIBRARY=${PYTHON_LIBRARY}
     -Dlinalg_ROOT=${CMAKE_CURRENT_SOURCE_DIR}
     -Dpybind11_DIR=${pybind11_DIR}
     -Dxsimd_DIR=${xsimd_DIR}
     -Dxtl_DIR=${xtl_DIR}
     -Dxtensor_DIR=${xtensor_DIR}
     -Dxtensor-blas_DIR=${xtensor-blas_DIR}
     -Dxtensor-io_DIR=${xtensor-io_DIR}
     -Dxtensor-python_DIR=${xtensor-python_DIR}
   BUILD_ALWAYS
     1
  )
