# GB Emulator Project Overview

## What is this?
A simple Game Boy emulator written in modern C++23, targeting the classic 8-bit Game Boy (DMG) architecture.

## Project Structure
```
gb-emulator/
├── main.cpp                    # Entry point (currently a stub)
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
- **Compiler**: Clang 20+
- **C++ Standard**: C++23
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

1. **Read the `docs/` folder first** — all documentation is numbered and should be read in order
2. **Always make incremental changes** — small, focused commits
3. **Always use TDD** — write the failing test first, then implement
4. **Build and test frequently** — use `cmake --build --preset conan-debug` and `ctest --preset conan-debug`
5. **Do not read toolchain/CMake toolchain code** unless explicitly asked — it is infrastructure and not part of the emulator logic

## Architecture Summary
The emulator follows a device-interconnect model:
- **AddressBus** routes read/write operations to connected devices via `AddressBusAddressable` interface
- **CPU** contains registers, opcode dispatch, and uses coroutines for multi-cycle instruction execution
- **Cartridge** loads and parses ROM files (header validation, Nintendo logo check)
- Instruction handlers are implemented as C++23 coroutines using `co_yield` for multi-cycle timing and `co_return` for completion

## Key Conventions
- All files include MIT license header: `Copyright © 2026. Tim Herreijgers`
- Namespace: `EmulatorLib` for emulator code, `UtilityLib` for utilities
- Byte literals: use `_b` suffix (e.g., `0x00_b`)
- Integer literals: use `_u8`, `_u16`, `_s8`, `_s16` suffixes
- All instruction handlers follow `Execute<Name>` naming
- Header guards: `#pragma once`
