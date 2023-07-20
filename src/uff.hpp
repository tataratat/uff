#pragma once

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

/// @brief
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
  std::sort(idx.begin(), idx.end(),
            [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });

  return idx;
}

template <typename DataType>
inline void uff(DataType* original_points, /* in */
                int& number_of_points,     /* in */
                int& point_dim,            /* in */
                DataType* metric,          /* in */
                DataType& tolerance,       /* in */
                const bool& stable,        /* in */
                DataType* new_points,      /* out */
                int* new_indices,          /* out */
                int& number_of_new_points, /* out */
                int* inverse) {            /* out */

  const DataType tolerance_squared{tolerance * tolerance};

  // Create a vector that contains the metric
  std::vector<DataType> vector_metric{};
  vector_metric.resize(number_of_points);
  for (int i{0}; i < number_of_points; i++) {
    vector_metric[i] = metric[0] * original_points[i * point_dim];
    for (int j{1}; j < point_dim; j++) {
      vector_metric[i] += metric[j] * original_points[i * point_dim + j];
    }
  }

  // Sort Metric Vector
  const auto metric_order_indices = sort_indices(vector_metric);

  // Reallocate new vector, set to -1 to mark untouched
  std::vector<int> stable_inverse;
  std::vector<bool> newpointmasks(number_of_points);  // zero (false) init
  std::fill(inverse, inverse + number_of_points, -1);

  // Loop over points
  number_of_new_points = 0;
  for (unsigned int lower_limit{0};
       lower_limit < metric_order_indices.size() - 1; lower_limit++) {
    // Point already processed
    if (inverse[metric_order_indices[lower_limit]] != -1) {
      continue;
    } else {
      newpointmasks[metric_order_indices[lower_limit]] = true;
    }

    // Point has not been processed -> add it to new point list
    if (!stable) {
      for (int i_dim{0}; i_dim < point_dim; i_dim++) {
        new_points[number_of_new_points * point_dim + i_dim] =
            original_points[metric_order_indices[lower_limit] * point_dim +
                            i_dim];
        new_indices[number_of_new_points] = metric_order_indices[lower_limit];
      }
    }
    inverse[metric_order_indices[lower_limit]] = number_of_new_points;

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
        inverse[metric_order_indices[upper_limit]] = number_of_new_points;
        newpointmasks[metric_order_indices[upper_limit]] = false;
        // If stable, the index with the lower id needs to be stored
        if ((stable) && (metric_order_indices[upper_limit] <
                         metric_order_indices[lower_limit])) {
          newpointmasks[metric_order_indices[upper_limit]] = true;
          newpointmasks[metric_order_indices[lower_limit]] = false;
        }
      }
      upper_limit++;
      if (upper_limit >= metric_order_indices.size()) {
        break;
      }
    }
    number_of_new_points++;
  }

  // Special case
  const auto& last_index = metric_order_indices.size() - 1;
  if (inverse[metric_order_indices[last_index]] == -1) {
    if (!stable) {
      for (int i_dim{0}; i_dim < point_dim; i_dim++) {
        new_points[number_of_new_points * point_dim + i_dim] =
            original_points[metric_order_indices[last_index] * point_dim +
                            i_dim];
        new_indices[number_of_new_points] = metric_order_indices[last_index];
      }
    }
    inverse[metric_order_indices[last_index]] = number_of_new_points;
    number_of_new_points++;
    newpointmasks[metric_order_indices[last_index]] = true;
  } else {
    newpointmasks[metric_order_indices[last_index]] = false;
  }

  if (stable) {
    int counter{};
    stable_inverse.assign(number_of_new_points, -1);
    for (int i{0}; i < number_of_points; i++) {
      if (newpointmasks[i]) {
        for (int j{0}; j < point_dim; j++) {
          new_points[counter * point_dim + j] =
              original_points[i * point_dim + j];

          new_indices[counter] = i;
        }
        stable_inverse[inverse[i]] = counter;
        counter++;
      }
      inverse[i] = stable_inverse[inverse[i]];
    }
  }

} /* uff */

}  // namespace uff
