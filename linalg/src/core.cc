#include <pybind11/pybind11.h>

namespace py = pybind11;

void export_linalg(py::module &);

PYBIND11_MODULE(core, m) {
  m.doc() = "Test module for xtensor python bindings";

  export_linalg(m);
}
