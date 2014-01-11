cppkit
======

XXX - cppkit is a work in progress, and is currently in a VERY alpha state. Use at your own risk.

cppkit is a posix / win32 portability library that also aims to fill in a few cracks in C++11. Our primary focus is on
ease of use. Our goal is that developers familiar with higher level languages will be comfortable with cppkit.

compilation
===========

cppkit uses cmake. To compile and install the library type the following from the top level cppkit directory:

       mkdir build
       cd build
       cmake ..
       make
       sudo make install

To build and run the unit tests type the following from the "ut" directory:

       mkdir build
       cd build
       cmake ..
       make
       LD_LIBRARY_PATH=/usr/local/lib ./ut

