import uffpy as uff
import numpy as np

a = np.random.rand(10,3)
a[1,:] = a[3,:]
a[4,:] = a[7,:]
i0,i1,i2 = uff.uffpy(a, 1e-5, False)
print(a)
print(i0)
print(i1)
print(i2)
print(np.allclose(a, i0[i2]))

i0,i1,i2 = uff.uffpy(a, 1e-5, True)
print(a)
print(i0)
print(i1)
print([0,1,2,1,3,4,5,3,6,7])
print(i2)
