from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake


class GBEmulatorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("gtest/1.17.0")
        self.requires("benchmark/1.9.4")

    def build_requirements(self):
        self.tool_requires("llvm-toolchain/20.1.0")
        self.tool_requires("cmake/[>=4.2.0]")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def configure(self):
        pass
