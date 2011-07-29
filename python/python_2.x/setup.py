from distutils.core import *
from subprocess import *

from distutils.core import setup
from distutils.extension import Extension

setup(
    name='apporo',
    version='0.0.1',
    description = 'Apporo : Approximate String Matching Engine',
    author = 'Toshinori Sato',
    author_email = 'apporo@googlecode.com',
    url = 'http://code.google.com/p/apporo',
    ext_modules=[Extension('apporo', ['apporo_python.cc'], libraries=['apporo'])],
    py_modules=['apporo_python'],
    )
