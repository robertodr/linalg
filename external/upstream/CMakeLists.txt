# Note: order is important as some projects might be interdependent
foreach(dir
  pybind11
  xsimd
  xtl
  xtensor
  xtensor-blas
  xtensor-io
  xtensor-python
  )
  add_subdirectory(${dir})
endforeach()
