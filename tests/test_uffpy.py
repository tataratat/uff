import unittest

import numpy as np

import uffpy


class UFFTest(unittest.TestCase):
    def test_uff(self):
        # make random array where (1, 3) and (4, 7) entries are the same
        # respectively
        a = np.random.rand(10, 3)
        a[1, :] = a[3, :]
        a[4, :] = a[7, :]

        # stable = false
        new_points, new_point_masks, inverse = uffpy.uffpy(a, 1e-5, False)

        # inverse with new_points should reconstruct original input
        assert np.allclose(a, new_points[inverse])

        # stable = true
        new_points, new_point_marks, inverse = uffpy.uffpy(a, 1e-5, True)

        # inverse should reference unique id that first appears
        assert np.alltrue(inverse == [0, 1, 2, 1, 3, 4, 5, 3, 6, 7])

        # original input reconstruction
        assert np.allclose(a, new_points[inverse])

        dim = 3
        test_values = 100
        test_vector = np.random.rand(test_values, dim)
        test_vector = np.vstack(
            [
                test_vector,
                test_vector,
                np.random.rand(test_values, dim),
                test_vector,
            ]
        )
        new_points, mask, inverse = uffpy.uffpy(test_vector, 1e-12, False)

        assert np.allclose(new_points, test_vector[mask])
        assert np.allclose(new_points[inverse, :], test_vector)
        assert new_points.shape[0] == test_values * 2

        new_points, mask, inverse = uffpy.uffpy(test_vector, 1e-12, False)

        assert np.allclose(new_points, test_vector[mask])
        assert np.allclose(new_points[inverse, :], test_vector)
        assert new_points.shape[0] == 2 * test_values

        # Test without
        test_vector = np.random.rand(test_values, dim)
        test_vector = np.vstack(
            [test_vector, test_vector, np.random.rand(test_values, dim)]
        )
        new_points, mask, inverse = uffpy.uffpy(test_vector, 1e-12, True)

        assert np.allclose(new_points, test_vector[mask])
        assert np.allclose(new_points[inverse, :], test_vector)
        assert new_points.shape[0] == 2 * test_values

        new_points, mask, inverse = uffpy.uffpy(test_vector, 1e-12, False)

        assert np.allclose(new_points, test_vector[mask])
        assert np.allclose(new_points[inverse, :], test_vector)
        assert new_points.shape[0] == 2 * test_values

        # Test without duplicates
        test_vector = np.random.rand(test_values, dim)
        test_vector = np.vstack(
            [test_vector, np.random.rand(test_values, dim)]
        )
        new_points, mask, inverse = uffpy.uffpy(test_vector, 1e-12, True)

        assert np.allclose(new_points, test_vector[mask])
        assert np.allclose(new_points[inverse, :], test_vector)
        assert new_points.shape[0] == 2 * test_values

        new_points, mask, inverse = uffpy.uffpy(test_vector, 1e-12, False)

        assert np.allclose(new_points, test_vector[mask])
        assert np.allclose(new_points[inverse, :], test_vector)
        assert new_points.shape[0] == 2 * test_values


if __name__ == "__main__":
    unittest.main()
