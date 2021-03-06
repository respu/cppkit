#!/bin/bash

# ./build_lib.sh <name> <src> <path>

echo "Building: $1"

pushd $3

if [ -d "build" ]; then
    rm -rf build
fi

mkdir build

pushd build

if [ $4 = "RELEASE" ]; then
    cmake -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_BUILD_TYPE=Release .. -DADDITIONAL_RELATIVE_RPATH=../SharedLibs/libs
    cmake --build . --config Release
    cmake --build . --target package
else
    cmake -DCMAKE_VERBOSE_MAKEFILE=ON .. -DADDITIONAL_RELATIVE_RPATH=../SharedLibs/libs
    cmake --build .
    cmake --build . --target package
fi

popd

popd
