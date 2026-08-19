# Context File — GB Emulator Instruction Tests

## 1. Project Overview

**Location:** `/Users/timherreijgers/development/c++/gb-emulator`

A Game Boy emulator written in C++. The project has a layered structure:

- `emulatorlib/` — Core emulator library (source, headers, tests)
- `utilitylib/` — Shared utilities (byte types, etc.)
- `build/` — CMake build output (Debug/Release presets)

### Key Directories for Tests

| Path                                    | Purpose                                    |
|-----------------------------------------|--------------------------------------------|
| `emulatorlib/test/instructions_tests/`  | Instruction-level unit tests (.cpp + .h)   |
| `emulatorlib/include/emulatorlib/test/` | Test infrastructure headers (mocks, stubs) |
| `emulatorlib/src/instruction_handlers/` | Instruction handler implementations        |

## 2. Build & Test Commands

### Build

```bash
cd /Users/timherreijgers/development/c++/gb-emulator
cmake --build --preset conan-debug
```

### Run Tests

```bash
cd /Users/timherreijgers/development/c++/gb-emulator/build/Debug
./emulatorlib/test/emulatorlibTest
```

### Filter by Test Class or Name

```bash
# Run only LoadAFromIndirectA16Test
./emulatorlib/test/emulatorlibTest --gtest_filter="*LoadAFromIndirectA16*"

# Run a specific test within a class
./emulatorlib/test/emulatorlibTest --gtest_filter="LoadAFromIndirectA16Test.ExecutingOpCode"
```

## 3. Test Infrastructure

### InstructionTestBase (the base class all instruction tests should inherit)

**Location:** `emulatorlib/test/instructions_tests/instruction_test_base.h`

```cpp
class InstructionTestBase : public ::testing::Test
{
protected:
    testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    ProgramStub m_program{0x100};          // ← serves instruction bytes from offset 0x100
    AddressBus m_bus{{m_addressableMock, m_program}};
    Cpu m_cpu{m_bus};
};
```

This provides three protected members to every test:

- **`m_addressableMock`** — A nice mock implementing `AddressBusAddressable`. Use `EXPECT_CALL` and `ON_CALL` on its
  `ReadFromAddress()` / `WriteToAddress()` methods.
- **`m_program`** — A `ProgramStub` at memory offset 0x100. Call `m_program.WriteProgram({byte0, byte1, byte2, ...})` to
  define instruction bytes that the CPU will fetch from addresses 0x0100, 0x0101, etc.
- **`m_cpu`** — A fully wired-up CPU (with bus + mock) ready to `Step()` through instructions.

### ProgramStub (how programs are loaded)

**Location:** `emulatorlib/include/emulatorlib/test/program_stub.h`

The stub maps reads from its offset range to a user-defined byte sequence:

```cpp
m_program.WriteProgram({0xF0_b, 0xAA_b, 0x00_b});
// Reads from address 0x0100 → 0xF0
// Reads from address 0x0101 → 0xAA
// Reads from address 0x0102 → 0x00
```

Reads from addresses outside the program return `0x00_b`. Writes are a no-op.

### AddressBusAddressableMock (the memory/bus mock)

**Location:** `emulatorlib/include/emulatorlib/test/address_bus_addressable_mock.h`

Exposes two methods for mocking:

- `ReadFromAddress(uint16_t)` — returns the byte read from that address
- `WriteToAddress(uint16_t, std::byte)` — records writes (useful to verify no unintended writes)

## 4. CPU.step () Behavior (critical for test structure)

**Source:** `emulatorlib/src/cpu.cpp`

Each call to `m_cpu.Step()` does two things:

1. **Resumes the previous instruction's coroutine** (if not done). If it still has more steps, return early — no new
   byte is fetched.
2. **If the previous instruction is complete:**
    - Read next byte from bus → store in `instructionRegister`
    - Increment program counter
    - Dispatch to the handler coroutine for that opcode

**Implication for tests:** The first `Step()` after a `SetUp()` will read a *new* instruction byte. Subsequent `Step()`
calls resume the instruction's coroutine (which may have multiple `co_yield` points). When `m_cpu.Step()` returns and
the coroutine is done, the next call reads a new opcode.

## 5. The Pattern for Instruction Tests

### Before (OLD — do NOT use)

```cpp
#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu.h"
#include "emulatorlib/instruction_translation.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"
#include "utilitylib/byte_utils.h"
#include "instruction_handlers/register_io_helpers.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{
class LoadAFromIndirectA16Test : public ::testing::Test
{
protected:
    ::testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    AddressBus m_bus{{m_addressableMock}};
    Cpu m_cpu{m_bus};
};

TEST_F(LoadAFromIndirectA16Test, ExecutingOpCode)
{
    ON_CALL(m_addressableMock, ReadFromAddress(0x0100)).WillByDefault(::testing::Return(0xFA_b));
    ON_CALL(m_addressableMock, ReadFromAddress(0x0101)).WillByDefault(::testing::Return(0xBB_b));
    // ... manual mocking everywhere
}
```

### After (NEW — the correct pattern)

```cpp
#include "instruction_test_base.h"

namespace EmulatorLib::Test
{

class LoadAFromIndirectA8Test : public InstructionTestBase
{
protected:
    void SetUp() override
    {
        m_program.WriteProgram({0xF0_b, 0xAA_b, 0x00_b});
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_F(LoadAFromIndirectA8Test, ExecutingOpCode)
{
    const auto initialValue = m_cpu.Registers().accumulator.value;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0xF0_b));
    ASSERT_THAT(m_cpu.Registers().accumulator, ::testing::Eq(initialValue));

    EXPECT_CALL(m_addressableMock, WriteToAddress(::testing::_, ::testing::_)).Times(0);
    m_cpu.Step();
    // ... continue stepping and asserting
}

} // namespace EmulatorLib::Test
```

### Key Conversions Checklist

1. **Include** → Replace all manual includes with just `#include "instruction_test_base.h"` (and optionally
   `utilitylib/byte_utils.h` for `_b` suffix).
2. **Class declaration** → Change `: public ::testing::Test` to `: public InstructionTestBase`. Remove manual
   mock/bus/cpu member declarations.
3. **SetUp ()** → Add a `SetUp()` override with:
    - `m_program.WriteProgram({opcodes...});` to define the instruction bytes
    -
   `EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF))).WillRepeatedly(::testing::Return(0x00_b));`
   as a catch-all for any read that doesn't have an explicit expectation (prevents "Uninteresting function call"
   failures).
4. **Test body** → Replace `ON_CALL(m_addressableMock, ReadFromAddress(...))` patterns with direct expectations where
   needed. Use the program stub to serve opcodes instead of mocking individual addresses.

#### Comment style (do not add boilerplate)

- Do not add explanatory/narrative comments to these tests; follow the existing minimal-comment style.
- Avoid comments such as:
  - `// Program: 0xF2 (LD A,(C)), then 0x00 (NOP)`
  - `// Catch-all for unmapped reads below ROM upper bound to avoid uninteresting calls`
  - `// No writes should occur during this instruction`
  - `// Next step should fetch the following opcode (0x00)`
- Tests should be self-explanatory through class/test names, `SetUp()`, expectations, and assertions. Only add a brief
  comment if it disambiguates genuinely non-obvious behavior.
- Preserve existing blank lines used as visual separation in the original tests; keep that spacing when converting or
  cleaning up.

## 6. Register Types for Assertions

Registers are available via `m_cpu.Registers()`:

| Field                                    | Type                         | Notes                                     |
|------------------------------------------|------------------------------|-------------------------------------------|
| `instructionRegister`                    | Register8Bit                 | Currently executing opcode                |
| `accumulator`                            | Register8Bit                 | A register                                |
| `zRegister`                              | Register8Bit                 | Lower byte of wz (temporary)              |
| `wRegister`                              | Register8Bit                 | Upper byte of wz                          |
| `bcRegister` / `bRegister` / `cRegister` | Register16Bit / Register8Bit | BC pair                                   |
| `deRegister` / `dRegister` / `eRegister` | Register16Bit / Register8Bit | DE pair                                   |
| `hlRegister` / `hRegister` / `lRegister` | Register16Bit / Register8Bit | HL pair                                   |
| `wzRegister`                             | Register16Bit                | Combined w+z (used for 16-bit addressing) |
| `programCounter`                         | Register16Bit                | Current program counter                   |
| `stackPointer`                           | Register16Bit                | Stack pointer                             |
| `flags`                                  | Register8Bit                 | Flags register                            |

### Assertion style

```cpp
ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xBB_b));
```

For value comparison use `.value`:

```cpp
m_cpu.Registers().accumulator = 0x0F_b;   // assignment
const auto val = m_cpu.Registers().accumulator.value;  // read raw uint8_t
```

## 7. Instruction Handler Reference (load_a_from_indirect variants)

### ExecuteLoadAFromIndirectA8 (opcode pattern: `LD A, (n)`) — loads from address 0xFF00 + n

**Handler:** `emulatorlib/src/instruction_handlers/load_a_from_indirect_address.h`

1. `Step()` — reads PC→address into wzRegister → increments PC
2. `Step()` — reads from address (0xFF00 | low_byte of wz) into zRegister → increments PC
3. After coroutine completes: accumulator = zRegister

### ExecuteLoadAFromIndirectA16 (opcode pattern: `LD A, (nn)`) — loads from 16-bit address in memory

**Handler:** same file, below the above function

1. `Step()` — reads PC→address into wzRegister → increments PC
2. `Step()` — reads next byte into wRegister → increments PC (so wz now has the full 16-bit addr)
3. After coroutine: accumulator = zRegister (loaded from wz address)

### w/z Register Behavior

`wzRegister` is a combined `Register16Bit`. Its upper byte is accessible via `wRegister`, lower byte via `zRegister`.
Writing to one updates the other appropriately through the backing 16-bit storage.

## 8. Files Already Using the Pattern (reference these, don't modify)

- `emulatorlib/test/instructions_tests/load_a_from_indirect_a8_test.cpp` — already converted
- `emulatorlib/test/instructions_tests/load_a16_sp_test.cpp` — already converted
- `emulatorlib/test/instructions_tests/load_a_from_indirect_a16_test.cpp` — already converted

## 9. Files Still Needing Conversion (the remaining ones)

To find which tests still use the old pattern, grep for classes that inherit `::testing::Test` instead of
`InstructionTestBase`:

```bash
grep -l ": public ::testing::Test" emulatorlib/test/instructions_tests/*.cpp
```

For each one:

1. Read the test to understand what it's testing
2. Read the corresponding instruction handler in `emulatorlib/src/instruction_handlers/` to know how many CPU steps are
   needed and which registers are affected
3. Rewrite using the pattern described in Section 5

### Common patterns to watch for in handlers

- Handlers use C++20 coroutines (`co_yield` / `co_return`)
- Each `co_yield` = one additional `Step()` call from the test's perspective
- After a `co_return`, the instruction is done and the next `Step()` will read a new opcode

## 10. Quick Conversion Workflow

For each remaining test file:

1. `cat emulatorlib/test/instructions_tests/<file>.cpp` — read current state
2. `cat emulatorlib/src/instruction_handlers/<handler>.h` — check how many steps the instruction takes
3. Rewrite the file using Section 5 pattern
4. `cd build/Debug && ./emulatorlib/test/emulatorlibTest --gtest_filter="*<ClassName>*"` — run to verify
