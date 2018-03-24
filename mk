#!/bin/sh

mkdir -p build && cd build && cmake ../src && make -j 6
