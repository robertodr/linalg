find_package(xtensor-io 0.7.0 CONFIG QUIET)

if(xtensor-io_FOUND)
  message(STATUS "${Cyan}Found xtensor-io${ColourReset}: ${xtensor-io_INCLUDE_DIR} (found version ${xtensor-io_VERSION})")
  add_library(xtensor-io_external INTERFACE)  # dummy
else()
  include(ExternalProject)
  message(STATUS "Suitable xtensor-io could not be located, ${Magenta}Building xtensor-io${ColourReset} instead.")
  ExternalProject_Add(xtensor-io_external
    DEPENDS
      xtl_external
      xtensor_external
    URL
      https://github.com/QuantStack/xtensor-io/archive/0.7.0.zip
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} # Unused, but probed by xtensor-io's CMakeLists.txt
      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
      -DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_INCLUDEDIR}
      -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
      -Dxtl_DIR=${xtl_DIR}
      -Dxtensor_DIR=${xtensor_DIR}
      -DXTENSOR_IO_CMAKECONFIG_INSTALL_DIR=share/cmake/xtensor-io # Wrong in upstream CMakeLists.txt
      )

  set(xtensor-io_DIR ${STAGED_INSTALL_PREFIX}/share/cmake/xtensor-io CACHE PATH "path to internally built xtensor-ioConfig.cmake" FORCE)
endif()
