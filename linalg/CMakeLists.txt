cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(linalg-core
        LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${linalg_ROOT}/cmake)

include(GNUInstallDirs)
include(custom_color_messages)

#  <<  Pybind11 & Python  >>
if(MSVC)
  set(PYBIND11_CPP_STANDARD "/std:c++${CMAKE_CXX_STANDARD}")
else()
  set(PYBIND11_CPP_STANDARD "-std=c++${CMAKE_CXX_STANDARD}")
endif()
find_package(pybind11 2.2.3 CONFIG REQUIRED)
message(STATUS "${Cyan}Using pybind11${ColourReset}: ${pybind11_INCLUDE_DIR} (version ${pybind11_VERSION} for Py${PYTHON_VERSION_STRING} and ${PYBIND11_CPP_STANDARD})")
message(STATUS "${Cyan}Using Python ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}${ColourReset}: ${PYTHON_EXECUTABLE}")

# <<< The whole xtensor stack >>>

find_package(xsimd 7.1.3 CONFIG REQUIRED)
get_property(_loc TARGET xsimd PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "${Cyan}Using xsimd${ColourReset}: ${_loc} (version ${xsimd_VERSION})")

find_package(xtl 0.5.4 CONFIG REQUIRED)
get_property(_loc TARGET xtl PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "${Cyan}Using xtl${ColourReset}: ${_loc} (version ${xtl_VERSION})")

find_package(xtensor 0.19.4 CONFIG REQUIRED)
get_property(_loc TARGET xtensor PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "${Cyan}Using xtensor${ColourReset}: ${_loc} (version ${xtensor_VERSION})")

find_package(xtensor-blas 0.15.2 CONFIG REQUIRED)
message(STATUS "${Cyan}Using xtensor-blas${ColourReset}: ${_loc} (version ${xtensor-blas_VERSION})")

find_package(xtensor-io 0.7.0 CONFIG REQUIRED)
message(STATUS "${Cyan}Using xtensor-io${ColourReset}: ${_loc} (version ${xtensor-io_VERSION})")

find_package(xtensor-python 0.22.1 CONFIG REQUIRED)
get_property(_loc TARGET xtensor-python PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "${Cyan}Using xtensor-python${ColourReset}: ${_loc} (version ${xtensor-python_VERSION})")
find_package(NumPy REQUIRED)

find_package(MKL REQUIRED)

add_subdirectory(src)
