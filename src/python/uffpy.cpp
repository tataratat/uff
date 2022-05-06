#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "../uff.hpp"


namespace py = pybind11;

/*!
 * PyUff
 *
 * Parameters
 * -----------
 * point: np.ndarray
 *
 * Returns
 * --------
 * unique_points_info: tuple
 *   (unique_points, unique_points_mask, inverse)
 */
py::tuple uffpy(py::array_t<double> points, double tolerance) {
    py::buffer_info p_buf = points.request();
    double* p_buf_ptr = static_cast<double *>(p_buf.ptr);
    int npoints = p_buf.shape[0];
    int pdim = p_buf.shape[1];
    // selecting metric for you.
    double* metric = new double[pdim];
    for (int i{0}; i < pdim; i++) {
        metric[i] = 1.;
    }

    // prepare output arrays
    py::array_t<int> np_newpointmasks(npoints);
    py::buffer_info npm_buf = np_newpointmasks.request();
    int* newpointmasks = static_cast<int *>(npm_buf.ptr);

    py::array_t<int> np_inverse(npoints);
    py::buffer_info i_buf = np_inverse.request();
    int* inverse = static_cast<int *>(i_buf.ptr);

    // prepare additional input vars
    int nnewpoints;

    // prepare temp array to store newpoints.
    // we call this thing phil
    double* phil = new double[npoints * pdim];

    // Dialogue
    // ---------
    // pyuff - (calls uff)
    // uff - Guten Tag uff am Apparat
    // pyuff - einmal uff bitte
    // uff - ::uff!
    uff::uff(p_buf_ptr,
             npoints,
             pdim,
             metric,
             tolerance,
             phil,
             newpointmasks,
             nnewpoints,
             inverse);

    // real newpoints
    py::array_t<double> np_newpoints({nnewpoints, pdim});
    py::buffer_info np_buf = np_newpoints.request();
    double* newpoints = static_cast<double *>(np_buf.ptr);
    // fill it up, phil.
    for (int i{0}; i < nnewpoints; i++) {
      for (int j{0}; j < pdim; j++) {
        newpoints[i*pdim + j] = phil[i*pdim + j];
      }
    }

    // be free and live your life
    delete[] metric;
    delete[] phil;

    return py::make_tuple(np_newpoints, np_newpointmasks, np_inverse);
}


PYBIND11_MODULE(uffpy, m) {
  m.def("uffpy",
            &uffpy,
            R"pbdoc(
              Given set of points, returns unique newpoints, unique point
              masks, inverse. Some useful information regarding returns:
              >>> newpoints, newpointmasks, inverse = uff.uff(queries, tol)
              >>> queries == newpoints[inverse]
              True
              >>> newpoints == queries[newpointmasks.astype(bool)]
              False

              Parameters
              -----------
              queries: (npoints, pdim) np.ndarray
              tolerance: float

              Returns
              --------
              unique_vertices_info: tuple
                (newpoints: np.ndarray (double),
                 newpointmasks: np.ndarray (int),
                 inverse: np.ndarray (int))
            )pbdoc",
            py::arg("queries"),
            py::arg("tolerance"));
}
