# Testing

## Test Framework
- Google Test (gtest 1.17.0) with Google Mock (gmock)
- All tests live in `*/test/` subdirectories

## Test Infrastructure

### Mock Objects (`emulatorlib/include/emulatorlib/test/`)
- **`AddressBusAddressableMock`** — Google Mock of `AddressBusAddressable` interface
  - `MOCK_METHOD(void, WriteToAddress, (uint16_t, std::byte), (override, noexcept))`
  - `MOCK_METHOD(std::byte, ReadFromAddress, (uint16_t), (const, override, noexcept))`

- **`ProgramStub`** — Simulates ROM/memory for instruction testing
  - Implements `AddressBusAddressable`
  - `WriteProgram({bytes...})` — loads program bytes
  - `ReadFromAddress(addr)` — returns byte from stored program at `addr - offset`
  - Returns `0x00_b` for out-of-bounds addresses

### Instruction Test Base (`emulatorlib/test/instructions_tests/instruction_test_base.h`)
```cpp
class InstructionTestBase : public ::testing::Test {
protected:
    testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    ProgramStub m_program{0x100};
    AddressBus m_bus{{m_addressableMock, m_program}};
    Cpu m_cpu{m_bus};
};
```

## Test Files

### Unit Tests (emulatorlib/test/)
| File | Tests |
|------|-------|
| `cpu_test.cpp` | CPU initialization, PC increment, opcode fetching |
| `address_bus_test.cpp` | Bus routing, OR-combining reads, write broadcasting |
| `cartridge_test.cpp` | ROM loading, data access |
| `cartridge_header_test.cpp` | Nintendo logo, title, cartridge type, ROM size, checksum validation |
| `masked_register_test.cpp` | Register mask enforcement, byte sub-register propagation |

### Instruction Tests (`emulatorlib/test/instructions_tests/`)
Parameterized tests for each instruction family:
| File | Instruction(s) |
|------|---------------|
| `load_r_r_test.cpp` | `LD r, r'` (all combinations) |
| `load_reg_n8_test.cpp` | `LD r, n` |
| `load_reg_n16_test.cpp` | `LD rr, nn` |
| `load_r_indirect_rr_test.cpp` | `LD r, (rr)` |
| `load_indirect_r_test.cpp` | `LD (r), r'` |
| `load_indirect_n8_test.cpp` | `LD (HL), n` |
| `load_indirect_a8_from_a_test.cpp` | `LD (a), A` |
| `load_indirect_a16_from_a_test.cpp` | `LD (aaaa), A` |
| `load_indirect_c_from_a_test.cpp` | `LD (C), A` |
| `load_a_from_indirect_a8_test.cpp` | `LD A, (a)` |
| `load_a_from_indirect_a16_test.cpp` | `LD A, (aaaa)` |
| `load_a_from_indirect_c_test.cpp` | `LD A, (C)` |
| `load_hl_from_sp_plus_e_test.cpp` | `LD HL, SP+e` |
| `load_sp_from_hl_test.cpp` | `LD SP, HL` |
| `load_a16_sp_test.cpp` | `LD (nn), SP` |
| `push_rr_test.cpp` | `PUSH rr` |
| `pop_rr_test.cpp` | `POP rr` |
| `add_r_test.cpp` | `ADD A, r` |
| `add_a_n8_test.cpp` | `ADD A, n` |
| `add_a_from_indirect_hl_test.cpp` | `ADD A, (HL)` |
| `adc_r_test.cpp` | `ADC A, r` |
| `adc_a_n8_test.cpp` | `ADC A, n` |
| `adc_a_from_indirect_hl_test.cpp` | `ADC A, (HL)` |
| `adc_a_test.cpp` | (alias for adc_r) |
| `sub_r_test.cpp` | `SUB r` |
| `sub_a_test.cpp` | (alias for sub_r / SUB A) |
| `sub_a_from_indirect_hl_test.cpp` | `SUB A, (HL)` |
| `inc_r_test.cpp` | `INC r` |
| `dec_r_test.cpp` | `DEC r` |
| `load_hl_indirect_increment_a_test.cpp` | `LD A, (HL+)` |
| `load_hl_indirect_decrement_a_test.cpp` | `LD A, (HL-)` |
| `load_indirect_8bit_test.cpp` | `LD (HL), n` |

### Test Pattern
Instruction tests typically follow this pattern:
1. Set up `EXPECT_CALL` or `ON_CALL` on the mock for bus operations
2. Write the instruction byte + operands to `m_program`
3. Set up register state
4. Call `m_cpu.Step()`
5. Assert final register state and instruction register

### Utility Tests (`utilitylib/test/`)
| File | Tests |
|------|-------|
| `add_with_carry_test.cpp` | AddWithCarry function |
| `sub_with_borrow_test.cpp` | SubWithBorrow function |
| `bit_mask_test.cpp` | BitMask constexpr |

## Building & Running Tests
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build
```

Or use CMake's built-in target:
```bash
cmake --build build --target gb_emulator_test
cmake --build build --target utilitylib_test
```
