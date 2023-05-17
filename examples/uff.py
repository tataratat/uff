import numpy as np

import uffpy

if __name__ == "__main__":
    nq = 10000
    dup = 234

    q = np.random.random((nq, 3))
    qq = np.vstack((q, q[:dup]))
    np.random.shuffle(qq)

    print("query points")
    print("===========================")
    print(qq)
    r = uffpy.uffpy(qq, 1e-5)
    print()
    print("results")
    print("========")
    print()
    print("newpoints")
    print("----------")
    print("shape: ", r[0].shape)
    print(r[0])
    print()
    print("newpointmasks")
    print("--------------")
    print(r[1])
    print()
    print("inverse")
    print("--------")
    print(r[2])
    print()

    # inverse with newpoints should match queries
    np.allclose(qq, r[0][r[2]])
    # duplicating points should have been removed
    assert r[1].sum() == nq
