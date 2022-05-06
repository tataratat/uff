# uff
_`U`ff, once upon a time, I've created a mesh with a lot of duplicating vertices and needed to `F`ind unique vertices `F`ast._

## Install guide
### c++ (minimal):
```
git clone git@github.com:tataratat/uff.git
cd uff
mkdir build
cd build
cmake -DBUILD_EXAMPLES=OFF -DBUILD_FORTRAN_MODULE=OFF -DCMAKE_BUILD_PREFIX=<path-to-install-dir> ..
(make)
make install
```
This makes function `uff` available from fortran side. For example:
```fortran
call uff(...)
```
However, if you prefer to have a separate fortran module, set `-DBUILD_FORTRAN_MODULE=ON`. This allows:
```fortran
use uff, only ufffortran

call ufffortran(...)
```

### python:
_option1: `pip`_
```
pip install uffpy
```
It works the best with `gcc` and `clang`. To make sure, you can set:
```
export CC=gcc # or clang
```
The newest version should always be available with:
```
pip install git+https://github.com/tataratat/uff.git@dev
```

_option2: DIY_
```
git clone git@github.com:tataratat/uff.git
cd uff
pip install -r requirements.txt
python3 setup.py install   # or `pip install .`
```
