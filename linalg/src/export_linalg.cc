#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include <functional>
#include <string>
#include <vector>

#define FORCE_IMPORT_ARRAY              // numpy C api loading
#include "xtensor-python/pytensor.hpp"  // Numpy bindings

#include "dimension.h"
#include "linalg.h"

using namespace psi;
namespace py = pybind11;
using namespace pybind11::literals;

namespace {
struct Rank1Decorator final {
    template <typename PyClass>
    void operator()(PyClass& cls, const std::string& /* name */) const {
        cls.def(py::init<const std::string&, const Dimension&>(), "Labeled, blocked vector", "label"_a, "dimpi"_a);
        cls.def(py::init<const std::string&, int>(), "Labeled, 1-irrep vector", "label"_a, "dim"_a);
        cls.def(py::init<const Dimension&>(), "Unlabeled, blocked vector", "dimpi"_a);
        cls.def(py::init<int>(), "Unlabeled, 1-irrep vector", "dim"_a);
        cls.def_property_readonly("dimpi", [](const typename PyClass::type& obj) { return obj.dimpi(); },
                                  py::return_value_policy::copy, "Return the Dimension object");
    }
};

struct Rank2Decorator final {
    template <typename PyClass>
    void operator()(PyClass& cls, const std::string& /* name */) const {
        cls.def(py::init<const std::string&, const Dimension&, const Dimension&, unsigned int>(),
                "Labeled, blocked, symmetry-assigned matrix", "label"_a, "rowspi"_a, "colspi"_a, "symmetry"_a);
        cls.def(py::init<const std::string&, const Dimension&, const Dimension&>(), "Labeled, blocked matrix",
                "label"_a, "rowspi"_a, "colspi"_a);
        cls.def(py::init<const std::string&, int, int>(), "Labeled, 1-irrep matrix", "label"_a, "rows"_a, "cols"_a);
        cls.def(py::init<const Dimension&, const Dimension&, unsigned int>(),
                "Unlabeled, blocked, symmetry-assigned matrix", "rowspi"_a, "colspi"_a, "symmetry"_a);
        cls.def(py::init<const Dimension&, const Dimension&>(), "Unlabeled, blocked matrix", "rowspi"_a, "colspi"_a);
        cls.def(py::init<int, int>(), "Unlabeled, 1-irrep matrix", "rows"_a, "cols"_a);
        cls.def_property_readonly("rowspi", [](const typename PyClass::type& obj) { return obj.rowspi(); },
                                  py::return_value_policy::copy, "Returns the rows per irrep array");
        cls.def("rows", [](const typename PyClass::type& obj, size_t h) { return obj.rows(h); },
                "Returns the number of rows in given irrep", "h"_a = 0);
        cls.def_property_readonly("colspi", [](const typename PyClass::type& obj) { return obj.colspi(); },
                                  py::return_value_policy::copy, "Returns the columns per irrep array");
        cls.def("cols", [](const typename PyClass::type& obj, size_t h) { return obj.cols(h); },
                "Returns the number of columns in given irrep", "h"_a = 0);
    }
};

template <size_t Rank>
struct RankNDecorator final {
    template <typename PyClass>
    void operator()(PyClass& cls, const std::string& name) const {
        cls.def(py::init<const std::string&, size_t, const std::array<Dimension, Rank>&, unsigned int>(),
                ("Labeled, blocked, symmetry-assigned " + name).c_str(), "label"_a, "blocks"_a, "axes_dimpi"_a,
                "symmetry"_a);
        cls.def(py::init<const std::string&, const std::array<Dimension, Rank>&>(),
                ("Labeled, 1-irrep " + name).c_str(), "label"_a, "axes_dimpi"_a);
        cls.def(py::init<size_t, const std::array<Dimension, Rank>&, unsigned int>(),
                ("Unlabeled, blocked, symmetry-assigned " + name).c_str(), "blocks"_a, "axes_dimpi"_a, "symmetry"_a);
        cls.def(py::init<size_t, const std::array<Dimension, Rank>&>(), ("Unlabeled, blocked " + name).c_str(),
                "blocks"_a, "axes_dimpi"_a);
        cls.def(py::init<const std::array<Dimension, Rank>&>(), ("Unlabeled, 1-irrep " + name).c_str(), "axes_dimpi"_a);
    }
};

template <typename T>
void declareMatrixFreeFunctions(py::module& mod) {
    mod.def("doublet", &doublet<T>,
            "Returns the multiplication of two matrices A and B, with options to transpose each beforehand", "A"_a,
            "B"_a, "transA"_a = false, "transB"_a = false);
}

template <typename T, size_t Rank>
struct DeclareTensor final {
    using Class = Tensor<T, Rank>;
    using PyClass = py::class_<Class, std::shared_ptr<Class>>;
    using SpecialBinder = std::function<void(PyClass&, const std::string&)>;

    static void bind_tensor(py::module& mod, const SpecialBinder& decorate) {
        std::string name = Class::pyClassName();

        PyClass cls(mod, name.c_str());

        cls.def_property_readonly("dim", &Class::dim, "Total number of elements");
        cls.def_property_readonly("nirrep", &Class::nirrep, "Number of irreps");
        cls.def_property("label", &Class::label, &Class::set_label, ("The label of the " + name).c_str());
        cls.def("axes_dimpi", &Class::axes_dimpi, "Returns the Dimension object for given axis", "axis"_a);
        cls.def_property_readonly("shapes", [](const Class& obj) { return obj.shapes(); },
                                  py ::return_value_policy::copy, "Shapes of blocks");
        cls.def_property("symmetry", &Class::symmetry, &Class::set_symmetry, ("The symmetry of " + name).c_str());

        cls.def("__repr__", &Class::repr);
        cls.def("__str__", &Class::str);
        cls.def("__format__", &Class::format, "extra"_a = "");
#if defined(PYBIND11_OVERLOAD_CAST)
        cls.def("__getitem__", py::overload_cast<size_t>(&Class::operator[]), "Return block at given irrep", "h"_a,
#else
        cls.def("__getitem__", static_cast<xt::xtensor<T, Rank>& (Class::*)(size_t)>(&Class::operator[]),
                "Return block at given irrep", "h"_a,
#endif
                py::is_operator(), py::return_value_policy::reference_internal);
        // FIXME Doesn't compile :/
        // cls.def("__setitem__", &Class::set_block, "Set block at given irrep", py::is_operator());
        cls.def("__setitem__", static_cast<xt::xtensor<T, Rank>& (Class::*)(size_t)>(&Class::operator[]),
                "Set block at given irrep", py::is_operator());

        // Rank-dependent bindings, e.g. CTORs
        decorate(cls, name);
    }
};
}  // namespace

void export_linalg(py::module& mod) {
    xt::import_numpy();
    // Rank-1 tensor, aka blocked vector
    auto decorate_v = Rank1Decorator();
    DeclareTensor<float, 1>::bind_tensor(mod, decorate_v);
    DeclareTensor<double, 1>::bind_tensor(mod, decorate_v);
    // Rank-2 tensor, aka blocked matrix
    auto decorate_m = Rank2Decorator();
    DeclareTensor<float, 2>::bind_tensor(mod, decorate_m);
    declareMatrixFreeFunctions<float>(mod);
    DeclareTensor<double, 2>::bind_tensor(mod, decorate_m);
    declareMatrixFreeFunctions<double>(mod);
    // Rank-3 tensor
    // FIXME not ideal to have rank info spread out...
    auto decorate_rankn = RankNDecorator<3>();
    DeclareTensor<float, 3>::bind_tensor(mod, decorate_rankn);
    DeclareTensor<double, 3>::bind_tensor(mod, decorate_rankn);
}
