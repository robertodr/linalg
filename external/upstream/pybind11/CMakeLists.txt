find_package(pybind11 2.2.3 CONFIG QUIET)

if(pybind11_FOUND)
  message(STATUS "${Cyan}Found pybind11${ColourReset}: ${pybind11_INCLUDE_DIR} (found version ${pybind11_VERSION})")
 add_library(pybind11_external INTERFACE)  # dummy
else()
  include(ExternalProject)
  message(STATUS "Suitable pybind11 could not be located, ${Magenta}Building pybind11${ColourReset} instead.")
  ExternalProject_Add(pybind11_external
    URL
      https://github.com/pybind/pybind11/archive/v2.2.3.tar.gz
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}  # ditto
      -DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_INCLUDEDIR}
      -DCMAKE_CXX_STANDARD=14
      -DPYBIND11_TEST=OFF
      )
  set(pybind11_DIR ${STAGED_INSTALL_PREFIX}/share/cmake/pybind11 CACHE PATH "path to internally built pybind11Config.cmake" FORCE)
endif()

