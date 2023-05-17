import unittest

import numpy as np

import uffpy as uff


class UFFTest(unittest.TestCase):
    def test_uff(self):
        # make random array where (1, 3) and (4, 7) entries are the same
        # respectively
        a = np.random.rand(10, 3)
        a[1, :] = a[3, :]
        a[4, :] = a[7, :]

        # stable = false
        new_points, new_point_masks, inverse = uff.uffpy(a, 1e-5, False)

        # inverse with new_points should reconstruct original input
        assert np.allclose(a, new_points[inverse])

        # stable = true
        new_points, new_point_marks, inverse = uff.uffpy(a, 1e-5, True)

        # inverse should reference unique id that first appears
        assert np.alltrue(inverse == [0, 1, 2, 1, 3, 4, 5, 3, 6, 7])

        # original input reconstruction
        assert np.allclose(a, new_points[inverse])


if __name__ == "__main__":
    unittest.main()
