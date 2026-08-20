#!/bin/bash

set -e

conan install -pr:a=clang-20-linux-x86_64 -s compiler.cppstd=23 -s build_type=Release -c tools.cmake.cmaketoolchain:generator="Ninja" . --build=missing
conan install -pr:a=clang-20-linux-x86_64 -s compiler.cppstd=23 -s build_type=RelWithDebInfo -c tools.cmake.cmaketoolchain:generator="Ninja" . --build=missing
conan install -pr:a=clang-20-linux-x86_64 -s compiler.cppstd=23 -s build_type=Debug -c tools.cmake.cmaketoolchain:generator="Ninja" . --build=missing
