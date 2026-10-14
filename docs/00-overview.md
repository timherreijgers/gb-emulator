# GB Emulator Project Overview

## What is this?

A simple Game Boy emulator written in modern C++26, targeting the classic 8-bit Game Boy (DMG) architecture.

## Project Structure

```
gb-emulator/
├── main.cpp                    # Entry point; loads a ROM and repeatedly runs Cpu::Step()
├── CMakeLists.txt              # Root CMake configuration
├── conanfile.py                # Conan package manager config
├── cmake/                      # CMake helper scripts (ccache)
├── emulatorlib/                # Core emulator library
│   ├── include/emulatorlib/    # Public headers
│   ├── src/                    # Source files + instruction handlers
│   └── test/                   # Tests
├── utilitylib/                 # Utility library (header-only)
│   ├── include/utilitylib/     # Public headers
│   └── test/                   # Tests
└── docs/                       # This documentation
```

## Build System

- **CMake** (≥ 4.2) with Conan package manager
- **Compiler**: Clang 22
- **C++ Standard**: C++26
- **Testing**: Google Test + Google Mock
- **Benchmarking**: Google Benchmark
- **Compiler flags**: `-Wall -Wextra -Wpedantic -Werror`

## Building

```bash
# Debug build
cmake --build --preset conan-debug
ctest --preset conan-debug
```

For other configurations (RelWithDebInfo, Release), adjust the preset accordingly.

## Important Guidelines

When working on this project:

1. **Read the relevant `docs/` files first** — documentation is numbered by topic
2. **Before writing any C++ code, read `docs/01-coding-style.md`** — it contains mandatory coding conventions
3. **Always make incremental changes** — small, focused commits
4. **Always use TDD** — write the failing test first, then implement
5. **Build and test frequently** — use `cmake --build --preset conan-debug` and `ctest --preset conan-debug`
6. **Do not read toolchain/CMake toolchain code** unless explicitly asked — it is infrastructure and not part of the
   emulator logic

## Architecture Summary

The emulator follows a device-interconnect model:

- **AddressBus** routes read/write operations to connected devices via `AddressBusAddressable` interface
- **CPU** contains registers, opcode dispatch, and uses coroutines to sequence instruction cycles; only a subset of DMG
  instructions is implemented
- **Cartridge** loads ROM bytes and checks ROM-size metadata; logo and header-checksum checks are separate queries, and
  MBC banking is not implemented
- **Ram** provides a fixed 8 KiB region at `0xC000–0xDFFF`; echo RAM and other memory-mapped devices are not implemented
- `main.cpp` loads a ROM and runs the CPU until an exception is raised; there is no display, audio, or complete
  console/peripheral emulation

## Key Conventions

- C++ source and header files use the MIT license header: `Copyright © 2026. Tim Herreijgers`
- Namespace: `EmulatorLib` for emulator code, `UtilityLib` for utilities
- Byte literals: use `_b` suffix (e.g., `0x00_b`)
- Integer literals: use `_u8`, `_u16`, `_s8`, `_s16` suffixes
- All instruction handlers follow `Execute<Name>` naming, except for UnimplementedOpcode and IllegalOpcode
- Header guards: `#pragma once`
