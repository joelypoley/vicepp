#!/bin/bash

set -e
set -x

pushd build

cmake --build .

bin/vice_test