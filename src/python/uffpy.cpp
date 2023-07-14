#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <iostream>

#include "../uff.hpp"

namespace py = pybind11;

/**
 * @brief
 *
 * @param points
 * @param tolerance
 * @param stable
 * @return py::tuple
 */
py::tuple uffpy(py::array_t<double> points, double tolerance, bool stable) {
  // Access points
  double* p_buf_ptr = static_cast<double*>(points.request().ptr);
  int npoints = points.shape(0);
  int pdim = points.shape(1);

  // selecting metric for you.
  std::vector<double> metric(pdim, 1.);

  // prepare output arrays
  py::array_t<int> np_newpointmasks(npoints);
  int* np_newpointmasks_ptr = static_cast<int*>(np_newpointmasks.request().ptr);

  py::array_t<int> np_inverse(npoints);
  py::buffer_info i_buf = np_inverse.request();
  int* inverse = static_cast<int*>(i_buf.ptr);

  // prepare additional input vars
  int nnewpoints{};

  // prepare temp array to store newpoints.
  // we call this thing phil
  py::array_t<double> np_newpoints({npoints, pdim});
  double* np_newpoints_ptr = static_cast<double*>(np_newpoints.request().ptr);

  uff::uff(p_buf_ptr,         // original points
           npoints,           // number of original points
           pdim,              // dimensions of points
           metric.data(),     // metric that reorganizes points
           tolerance,         // tolerance between neighboring points
           stable,            // Use stable sort
           np_newpoints_ptr,  // pointer to new points (make sure enough space
                              // is allocated)
           np_newpointmasks_ptr,  // mask to identify if a point is kept
           nnewpoints,            // number of new points
           inverse  // return value, inverse ids to original vector
  );

  // real newpoints
  assert(nnewpoints > 0);
  np_newpoints.resize({nnewpoints, pdim}, false);

  return py::make_tuple(np_newpoints, np_newpointmasks, np_inverse);
}

PYBIND11_MODULE(uffpy, m) {
  m.def("uffpy", &uffpy,
        R"pbdoc(
              Given set of points, returns unique newpoints, unique point
              masks, inverse. Some useful information regarding returns:
              >>> newpoints, np_newpointmasks_ptr, inverse = uff.uff(queries, tol)
              >>> queries == newpoints[inverse]
              True
              >>> newpoints == queries[np_newpointmasks_ptr.astype(bool)]
              False

              Parameters
              -----------
              queries: (npoints, pdim) np.ndarray
              tolerance: float

              Returns
              --------
              unique_vertices_info: tuple
                (newpoints: np.ndarray (double),
                 np_newpointmasks_ptr: np.ndarray (int),
                 inverse: np.ndarray (int))
            )pbdoc",
        py::arg("queries"), py::arg("tolerance"), py::arg("stable") = true);
}
