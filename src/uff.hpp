#pragma once

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

namespace uff {

/* util routines */
template <typename T>
inline T diff_norm_squared(const T* a, const size_t& start_a,
                           const size_t& start_b, const size_t& size) {
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
 *   original_points <- npoints * pdim
 *   metric <- point_dim
 *   newpointmasks <- npoints
 *   inverse <- npoints
 */
inline void uff(double* original_points, /* in */
                int& npoints,            /* in */
                int& point_dim,          /* in */
                double* metric,          /* in */
                double& tolerance,       /* in */
                const bool& stable,      /* in */
                double* newpoints,       /* out */
                int* newpointmasks,      /* out */
                int& nnewpoints,         /* out */
                int* inverse) {          /* out */

  const double tolerance_squared{tolerance * tolerance};

  // Create a vector that contains the metric
  std::vector<double> vector_metric{};
  vector_metric.resize(npoints);
  for (int i{0}; i < npoints; i++) {
    vector_metric[i] = metric[0] * original_points[i * point_dim];
    for (int j{1}; j < point_dim; j++) {
      vector_metric[i] += metric[j] * original_points[i * point_dim + j];
    }
  }

  // Sort Metric Vector
  const auto metric_order_indices = sort_indices(vector_metric);

  // Reallocate new vector, set to -1 to mark untouched
  std::vector<int> new_indices, stable_inverse;
  new_indices.assign(npoints, -1);

  // Loop over points
  nnewpoints = 0;
  for (unsigned int lower_limit{0};
       lower_limit < metric_order_indices.size() - 1; lower_limit++) {
    // Point already processed
    if (new_indices[metric_order_indices[lower_limit]] != -1) {
      continue;
    } else {
      newpointmasks[metric_order_indices[lower_limit]] = 1;
    }

    // Point has not been processed -> add it to new point list
    if (!stable) {
      for (int i_dim{0}; i_dim < point_dim; i_dim++) {
        newpoints[nnewpoints * point_dim + i_dim] =
            original_points[metric_order_indices[lower_limit] * point_dim +
                            i_dim];
      }
    }
    new_indices[metric_order_indices[lower_limit]] = nnewpoints;

    // Now check allowed range for duplicates
    unsigned int upper_limit = lower_limit + 1;
    while ((vector_metric[metric_order_indices[upper_limit]] -
            vector_metric[metric_order_indices[lower_limit]]) < tolerance) {
      const bool is_duplicate =
          diff_norm_squared(original_points,
                            metric_order_indices[lower_limit] * point_dim,
                            metric_order_indices[upper_limit] * point_dim,
                            point_dim) < tolerance_squared;
      if (is_duplicate) {
        new_indices[metric_order_indices[upper_limit]] = nnewpoints;
        newpointmasks[metric_order_indices[upper_limit]] = 0;
        // If stable, the index with the lower id needs to be stored
        if ((stable) && (metric_order_indices[upper_limit] <
                         metric_order_indices[lower_limit])) {
          newpointmasks[metric_order_indices[upper_limit]] = 1;
          newpointmasks[metric_order_indices[lower_limit]] = 0;
          stable_inverse[nnewpoints] = metric_order_indices[upper_limit];
        }
      }
      upper_limit++;
      if (upper_limit >= metric_order_indices.size()) {
        break;
      }
    }
    nnewpoints++;
  }

  // Special case
  const auto& last_index = metric_order_indices.size() - 1;
  if (new_indices[metric_order_indices[last_index]] == -1) {
    if (!stable) {
      for (int i_dim{0}; i_dim < point_dim; i_dim++) {
        newpoints[nnewpoints * point_dim + i_dim] =
            original_points[metric_order_indices[last_index] * point_dim +
                            i_dim];
      }
    }
    new_indices[metric_order_indices[last_index]] = nnewpoints;
    nnewpoints++;
    newpointmasks[metric_order_indices[last_index]] = 1;
  } else {
    newpointmasks[metric_order_indices[last_index]] = 0;
  }

  if (!stable) {
    // fill return buffer copy is sufficient
    for (int i{0}; i < npoints; i++) {
      inverse[i] = new_indices[i];
    }
  } else {
    int counter{};
    if (stable) {
      stable_inverse.assign(nnewpoints, -1);
    }
    for (int i{0}; i < npoints; i++) {
      if (newpointmasks[i] == 1) {
        for (int j{0}; j < point_dim; j++) {
          newpoints[counter * point_dim + j] =
              original_points[i * point_dim + j];
        }
        stable_inverse[new_indices[i]] = counter;
        counter++;
      }
      inverse[i] = stable_inverse[new_indices[i]];
    }
  }

} /* uff */

}  // namespace uff
