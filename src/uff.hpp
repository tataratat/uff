#pragma once

#include <vector>
#include <numeric>
#include <algorithm>


namespace uff {

/* util routines */
template <typename T>
inline T diff_norm_squared(const T* a,
                           const size_t& start_a,
                           const size_t& start_b,
                           const size_t& size) {
  T res{};
  for (unsigned int i{}; i < size; i++) {
    res +=
        (a[start_a + i] - a[start_b + i]) * (a[start_a + i] - a[start_b + i]);
  }
  return res;
}


/*
 * Sort Vector using lambda expressions
 * ref:
 *   stackoverflow.com/questions/1577475/c-sorting-and-keeping-track-of-indexes
 */
template <typename T>
std::vector<size_t> sort_indices(const std::vector<T>& v) {
  std::vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);
  std::stable_sort(idx.begin(), idx.end(),
                   [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });

  return idx;
}



/*!
 * Takes a random set of points and uniquifies the points using a metric
 * In theory this has complexity O(nlogn) whereas a KDTree has complexity 
 * O(n (logn)^dim). 
 *
 * Implementation adapted to be as primitive as possible.
 * Manage memory beforehand!
 * Buffer size hints:
 *   points <- npoints * pdim
 *   metric <- pdim
 *   newpoints <- npoints * pdim (may not use all. thus, nnewpoints)
 *   newpointmasks <- npoints
 *   inverse <- npoints
 */
void uff(double* points,     /* in */
         int& npoints,       /* in */
         int& pdim,          /* in */
         double* metric,     /* in */
         double& tolerance,  /* in */
         double* newpoints,  /* out */
         int* newpointmasks, /* out */
         int& nnewpoints,    /* out */
         int* inverse) {     /* out */

  const double tolerance_squared{tolerance * tolerance};

  // Create a vector that contains the metric
  std::vector<double> vector_metric{};
  vector_metric.resize(npoints);
  for (int i{0}; i < npoints; i++) {
    vector_metric[i] = metric[0] * points[i * pdim];
    for (int j{1}; j < pdim; j++) {
      vector_metric[i] +=
        metric[j]
        * points[i * pdim + j]; 
    }
  }

  // Sort Metric Vector
  const auto metric_order_indices = sort_indices(vector_metric);

  // Reallocate new vector
  std::vector<double> new_points{};
  new_points.reserve(npoints * pdim);
  std::vector<int> new_indices;
  new_indices.assign(npoints, -1);

  // Loop over points
  nnewpoints = 0;
  for (unsigned int lower_limit{0};
      lower_limit < metric_order_indices.size() - 1; lower_limit++) {
    // Point already processed
    if (new_indices[metric_order_indices[lower_limit]] != -1) {
        newpointmasks[metric_order_indices[lower_limit]] = 0;
        continue;
    } else {
        newpointmasks[metric_order_indices[lower_limit]] = 1;
    }

    // Point has not been processed -> add it to new point list
    for (int i_dim{0}; i_dim < pdim; i_dim++) {
      new_points.push_back(
          points[metric_order_indices[lower_limit]*pdim + i_dim]);
    }
    new_indices[metric_order_indices[lower_limit]] = nnewpoints;

    // Now check allowed range for duplicates
    unsigned int upper_limit = lower_limit + 1;
    while (
        (vector_metric[metric_order_indices[upper_limit]]
           - vector_metric[metric_order_indices[lower_limit]]) < tolerance
        && upper_limit < metric_order_indices.size()
    ) {
      const bool is_duplicate = diff_norm_squared(points, 
          metric_order_indices[lower_limit] * pdim,
          metric_order_indices[upper_limit] * pdim,
          pdim) < tolerance_squared;
      if (is_duplicate) {
        new_indices[metric_order_indices[upper_limit]] = nnewpoints;
      }
      upper_limit++;
    }
    nnewpoints++;
  }

  // Special case
  const auto &last_index = metric_order_indices.size() - 1;
  if (new_indices[metric_order_indices[last_index]] == -1) {
    for (int i_dim{0}; i_dim < pdim; i_dim++) {
      new_points.push_back(
          points[metric_order_indices[last_index]*pdim + i_dim]);
    }
    new_indices[metric_order_indices[last_index]] = nnewpoints;
    nnewpoints++;
    newpointmasks[metric_order_indices[last_index]] = 1;
  } else {
    newpointmasks[metric_order_indices[last_index]] = 0;
  }


  // fill return buffer
  for (int i{0}; i < npoints; i++) {
    if (i < nnewpoints) {
      for (int j{0}; j < pdim; j++) {
        newpoints[i*pdim + j] = new_points[i*pdim + j];
      }
    }
    inverse[i] = new_indices[i];
  }



} /* uff */

} /* namespace */
