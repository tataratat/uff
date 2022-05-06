from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup


with open("README.md", "r") as f:
    readme = f.read()

__version__ = "0.0.0"

ext_modules = [
    Pybind11Extension(
        "uffpy",
        ["src/python/uffpy.cpp"],
        extra_compile_args=["-O3"],
        cxx_std=11,
    )
]

setup(
    name="uffpy",
    version=__version__,
    description="Unique vertices finder",
    long_description=readme,
    long_description_content_type="text/markdown",
    author="Jaewook Lee",
    author_email="jaewooklee042@gmail.com",
    url="https://github.com/tataratat/uff",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    package_data={"src": ["*.hpp"]}, 
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Natural Language :: English',
        'Topic :: Scientific/Engineering'
    ],
    install_requires=[
        "numpy",
    ],
    zip_safe=False,
    license="MIT"
)
