# Build System

## Configuration

The project uses CMake 4.2 or newer with Conan-generated dependency and toolchain files. The Conan recipe provides Google Test 1.18.0, Google Benchmark 1.9.5, LLVM toolchain 22.1.8, and CMake 4.2 or newer. The configured C++ standard is C++26.

## Targets and Header File Sets

- `emulatorlib` is a static library. Its public headers are declared in the `interface_headers` `HEADERS` file set rooted at `emulatorlib/include`; implementation sources and private headers are listed separately.
- `utilitylib` is an interface library. Its public headers are declared in an `interface_headers` file set rooted at `utilitylib/include`.
- `emulatorlibTest` and the utility tests are added by their respective test subdirectories.
- Header lists use `file(GLOB_RECURSE ... CONFIGURE_DEPENDS ...)`, so CMake reconfigures when headers or sources are added or removed.

The root configuration enables `CMAKE_VERIFY_INTERFACE_HEADER_SETS` and `CMAKE_VERIFY_PRIVATE_HEADER_SETS`. The `run_all_header_verifications` target depends on CMake's `all_verify_header_sets` target and is included in the default build, ensuring public and private headers are checked independently for their declared dependencies.

## Compiler Warnings

All project targets receive these compiler options:

```text
-Wall -Wextra -Wpedantic -Werror -Wsuggest-override -Wshadow -Wswitch-enum -Wswitch-default -Wconversion
```

Warnings are treated as errors. The additional options check overrides, shadowing, enum and default switch handling, and implicit conversions.

## Build and Test

Use the configured Conan preset to build and run tests:

```bash
cmake --build --preset conan-debug
ctest --preset conan-debug
```

The default build also runs header-set verification through `run_all_header_verifications`.
