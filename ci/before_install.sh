#!/bin/bash

set -e

git clone --recursive -b master https://github.com/StanfordLegion/legion.git
pushd legion

mkdir build
pushd build

cmake ..
make
