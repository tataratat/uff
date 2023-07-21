#pragma once

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

/// @brief
namespace uff {

/* util routines */
template <typename T, typename IndexingType>
inline T diff_norm_squared(const T* a, const IndexingType& start_a,
                           const IndexingType& start_b,
                           const IndexingType& size) {
  T res{};
  for (IndexingType i{}; i < size; i++) {
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
template <typename IndexingType, typename T>
std::vector<IndexingType> sort_indices(const std::vector<T>& v) {
  std::vector<IndexingType> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);
  std::sort(idx.begin(), idx.end(),
            [&v](IndexingType i1, IndexingType i2) { return v[i1] < v[i2]; });

  return idx;
}

template <typename DataType, typename IndexingType>
inline void uff(DataType* original_poIndexingTypes,          /* in */
                IndexingType& number_of_poIndexingTypes,     /* in */
                IndexingType& poIndexingType_dim,            /* in */
                DataType* metric,                            /* in */
                DataType& tolerance,                         /* in */
                const bool& stable,                          /* in */
                DataType* new_poIndexingTypes,               /* out */
                IndexingType* new_indices,                   /* out */
                IndexingType& number_of_new_poIndexingTypes, /* out */
                IndexingType* inverse) {                     /* out */

  const DataType tolerance_squared{tolerance * tolerance};

  // Create a vector that contains the metric
  std::vector<DataType> vector_metric{};
  vector_metric.resize(number_of_poIndexingTypes);
  for (IndexingType i{0}; i < number_of_poIndexingTypes; i++) {
    vector_metric[i] =
        metric[0] * original_poIndexingTypes[i * poIndexingType_dim];
    for (IndexingType j{1}; j < poIndexingType_dim; j++) {
      vector_metric[i] +=
          metric[j] * original_poIndexingTypes[i * poIndexingType_dim + j];
    }
  }

  // Sort Metric Vector
  const auto metric_order_indices = sort_indices<IndexingType>(vector_metric);

  // Reallocate new vector, set to -1 to mark untouched
  std::vector<IndexingType> stable_inverse;
  std::vector<bool> newpoIndexingTypemasks(
      number_of_poIndexingTypes);  // zero (false) init
  std::fill(inverse, inverse + number_of_poIndexingTypes, -1);

  // Loop over poIndexingTypes
  number_of_new_poIndexingTypes = 0;
  for (IndexingType lower_limit{0};
       lower_limit < static_cast<IndexingType>(metric_order_indices.size()) - 1;
       lower_limit++) {
    // PoIndexingType already processed
    if (inverse[metric_order_indices[lower_limit]] != -1) {
      continue;
    } else {
      newpoIndexingTypemasks[metric_order_indices[lower_limit]] = true;
    }

    // PoIndexingType has not been processed -> add it to new poIndexingType
    // list
    if (!stable) {
      for (IndexingType i_dim{0}; i_dim < poIndexingType_dim; i_dim++) {
        new_poIndexingTypes[number_of_new_poIndexingTypes * poIndexingType_dim +
                            i_dim] =
            original_poIndexingTypes[metric_order_indices[lower_limit] *
                                         poIndexingType_dim +
                                     i_dim];
        new_indices[number_of_new_poIndexingTypes] =
            metric_order_indices[lower_limit];
      }
    }
    inverse[metric_order_indices[lower_limit]] = number_of_new_poIndexingTypes;

    // Now check allowed range for duplicates
    IndexingType upper_limit = lower_limit + 1;
    while ((vector_metric[metric_order_indices[upper_limit]] -
            vector_metric[metric_order_indices[lower_limit]]) < tolerance) {
      const bool is_duplicate =
          diff_norm_squared(
              original_poIndexingTypes,
              metric_order_indices[lower_limit] * poIndexingType_dim,
              metric_order_indices[upper_limit] * poIndexingType_dim,
              poIndexingType_dim) < tolerance_squared;
      if (is_duplicate) {
        inverse[metric_order_indices[upper_limit]] =
            number_of_new_poIndexingTypes;
        newpoIndexingTypemasks[metric_order_indices[upper_limit]] = false;
        // If stable, the index with the lower id needs to be stored
        if ((stable) && (metric_order_indices[upper_limit] <
                         metric_order_indices[lower_limit])) {
          newpoIndexingTypemasks[metric_order_indices[upper_limit]] = true;
          newpoIndexingTypemasks[metric_order_indices[lower_limit]] = false;
        }
      }
      upper_limit++;
      if (upper_limit >=
          static_cast<IndexingType>(metric_order_indices.size())) {
        break;
      }
    }
    number_of_new_poIndexingTypes++;
  }

  // Special case
  const auto& last_index = metric_order_indices.size() - 1;
  if (inverse[metric_order_indices[last_index]] == -1) {
    if (!stable) {
      for (IndexingType i_dim{0}; i_dim < poIndexingType_dim; i_dim++) {
        new_poIndexingTypes[number_of_new_poIndexingTypes * poIndexingType_dim +
                            i_dim] =
            original_poIndexingTypes[metric_order_indices[last_index] *
                                         poIndexingType_dim +
                                     i_dim];
        new_indices[number_of_new_poIndexingTypes] =
            metric_order_indices[last_index];
      }
    }
    inverse[metric_order_indices[last_index]] = number_of_new_poIndexingTypes;
    number_of_new_poIndexingTypes++;
    newpoIndexingTypemasks[metric_order_indices[last_index]] = true;
  } else {
    newpoIndexingTypemasks[metric_order_indices[last_index]] = false;
  }

  if (stable) {
    IndexingType counter{};
    stable_inverse.assign(number_of_new_poIndexingTypes, -1);
    for (IndexingType i{0}; i < number_of_poIndexingTypes; i++) {
      if (newpoIndexingTypemasks[i]) {
        for (IndexingType j{0}; j < poIndexingType_dim; j++) {
          new_poIndexingTypes[counter * poIndexingType_dim + j] =
              original_poIndexingTypes[i * poIndexingType_dim + j];

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
