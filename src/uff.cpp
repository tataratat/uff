#include "uff.hpp"

extern "C" {
/* Fortran */
void uff_(double* points,
          int* npoints,
          int* pdim,
          double* metric,
          double* tolerance,
          int* stable,
          double* newpoints,
          int* newpointmasks,
          int* nnewpoints,
          int* inverse) {

  bool stable_bool = static_cast<bool>(*stable);

  uff::uff(points,
           *npoints,
           *pdim,
           metric,
           *tolerance,
           stable_bool,
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
           int* stable,
           double* newpoints,
           int* newpointmasks,
           int* nnewpoints,
           int* inverse) {

  uff_(points,
       npoints,
       pdim,
       metric,
       tolerance,
       stable,
       newpoints,
       newpointmasks,
       nnewpoints,
       inverse);
};
}
