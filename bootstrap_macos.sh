#!/bin/bash

conan install -pr:a=clang-20-macos-armv8 -s compiler.cppstd=23 -s build_type=Release -c tools.cmake.cmaketoolchain:generator="Ninja" . --build=missing
conan install -pr:a=clang-20-macos-armv8 -s compiler.cppstd=23 -s build_type=RelWithDebInfo -c tools.cmake.cmaketoolchain:generator="Ninja" . --build=missing
conan install -pr:a=clang-20-macos-armv8 -s compiler.cppstd=23 -s build_type=Debug -c tools.cmake.cmaketoolchain:generator="Ninja" . --build=missing
