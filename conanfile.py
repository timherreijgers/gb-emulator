#  Copyright © 2026. Tim Herreijgers
#  Licensed using the MIT license

from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake, CMakeToolchain


class GBEmulatorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CMAKE_CXX_MODULE_STD"] = "ON"
        tc.generate()

    def requirements(self):
        self.requires("gtest/1.18.0")
        self.requires("benchmark/1.9.5")

    def build_requirements(self):
        self.tool_requires("llvm-toolchain/22.1.8")
        self.tool_requires("cmake/[>=4.3.0]")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def configure(self):
        pass
