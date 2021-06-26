#!/bin/bash

set -e
set -x

rm -rf build
mkdir build
pushd build

conan install .. --build=abseil --build=gtest --build=fmt --profile=clang
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

bin/vice