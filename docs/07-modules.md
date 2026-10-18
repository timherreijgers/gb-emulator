# C++ Modules and Build System

## Targets

- Root `CMakeLists.txt` requires CMake 4.2, enables C++ tests, adds `utilitylib` and `emulatorlib`, and links
  `gb_emulator` against `emulatorlib`.
- `utilitylib` is a static C++ module library. Its public `CXX_MODULES` file set contains
  `utilitylib/src/utilitylib.cppm` and the utility partitions; the primary interface re-exports the partitions.
- `emulatorlib` is a static library linked publicly to `utilitylib`. Its public `CXX_MODULES` file set contains the
  named interfaces in `emulatorlib/src/` (`emulatorlib.address_bus`, `emulatorlib.cartridge`, `emulatorlib.ram`, and
  `emulatorlib.cpu`) and CPU interface partitions. Instruction handlers under
  `emulatorlib/src/cpu/instruction_handlers/` are private implementation partitions, including the opcode map.
- The old `emulatorlib/include/emulatorlib/` and `utilitylib/include/utilitylib/` public header trees are gone.
  Include-based consumers must use module imports and link the corresponding CMake target instead.

## Module Organization

The `.cppm` files declare module interfaces and exported types; `.cpp` files outside the instruction-handler directory
provide implementations. The CPU's primary interface re-exports its register, flag, and instruction-support
partitions. Each module unit imports its direct dependencies explicitly: a private import in one partition is not
automatically visible in another. For example, the opcode-map partition imports `utilitylib` for its `std::byte`
formatter.

## Build and Test

Conan generates the CMake presets referenced by `CMakeUserPresets.json` under `build/Debug`, `build/Release`, and
`build/RelWithDebInfo`. Once dependencies and presets are configured, use:

```bash
cmake --build --preset conan-debug
ctest --preset conan-debug --output-on-failure
```

The other configurations use the corresponding `conan-release` or `conan-relwithdebinfo` preset. The tests are
registered as `EmulatorLibTest` and `UtilityLibTest`.