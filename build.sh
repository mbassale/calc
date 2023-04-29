#!/bin/sh

cmake -G Ninja -DLLVM_DIR=../build/lib/cmake/llvm -S . -B build/