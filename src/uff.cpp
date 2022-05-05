#include "uff.hpp"

extern "C" {
  /* Fortran */
  void uff_(double* points,
            int* npoints,
            int* pdim,
            double* metric,
            double* tolerance,
            double* newpoints,
            bool* newpointmasks,
            int* nnewpoints,
            int* inverse) {

    uff::uff(points,
             *npoints,
             *pdim,
             metric,
             *tolerance,
             newpoints,
             newpointmasks,
             *nnewpoints,
             inverse);
  };

  /* name untangling for fortran module */
  void ufff_(double* points,
             int* npoints,
             int* pdim,
             double* metric,
             double* tolerance,
             double* newpoints,
             bool* newpointmasks,
             int* nnewpoints,
             int* inverse) {

    uff_(points,
         npoints,
         pdim,
         metric,
         tolerance,
         newpoints,
         newpointmasks,
         nnewpoints,
         inverse);
  };
}
