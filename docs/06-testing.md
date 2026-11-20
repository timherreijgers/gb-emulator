# Testing

## Test Framework

- Google Test (gtest 1.18.0) with Google Mock (gmock)
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

### Instruction Test Base (`emulatorlib/test/src/instructions_tests/instruction_test_base.h`)

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

### Unit Tests (`emulatorlib/test/src/`)

| File                                                        | Tests                                                                                    |
|-------------------------------------------------------------|------------------------------------------------------------------------------------------|
| `cpu_test.cpp`                                              | CPU initialization (including disabled master interrupts), PC increment, opcode fetching |
| `instruction_handler_header_test.cpp`                       | Instruction handler header and coroutine support                                         |
| `instruction_translation.cpp` / `instruction_translation.h` | Test-only opcode-to-instruction-name mapping for parameterized tests                     |
| `address_bus_test.cpp`                                      | Bus routing, OR-combining reads, write broadcasting                                      |
| `cartridge_test.cpp`                                        | ROM loading, data access                                                                 |
| `cartridge_header_test.cpp`                                 | Nintendo logo, title, cartridge type, ROM size, checksum validation                      |
| `ram_test.cpp`                                              | Initial RAM contents and read/write access                                               |
| `masked_register_test.cpp`                                  | Register mask enforcement, including XOR assignment, and byte sub-register propagation   |
| `flag_helpers_test.cpp`                                     | Addition, subtraction, AND, OR, and XOR flag calculations                                |

### Instruction Tests (`emulatorlib/test/src/instructions_tests/`)

Parameterized tests for each instruction family:
| File | Instruction (s) |
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
| `add_hl_rr_test.cpp` | `ADD HL, rr` cycle timing and flags |
| `add_a_n8_test.cpp` | `ADD A, n` |
| `add_a_from_indirect_hl_test.cpp` | `ADD A, (HL)` |
| `adc_r_test.cpp` | `ADC A, r` |
| `adc_a_n8_test.cpp` | `ADC A, n` |
| `adc_a_from_indirect_hl_test.cpp` | `ADC A, (HL)` |
| `adc_a_test.cpp` | (alias for adc_r) |
| `sub_r_test.cpp` | `SUB r` |
| `sub_a_test.cpp` | (alias for sub_r / SUB A) |
| `sub_a_from_indirect_hl_test.cpp` | `SUB A, (HL)` |
| `sub_a_n8_test.cpp` | `SUB A, n` |
| `sbc_r_test.cpp` | `SBC A, r` |
| `sbc_a_test.cpp` | `SBC A, A` |
| `sbc_a_from_indirect_hl_test.cpp` | `SBC A, (HL)` |
| `sbc_a_n8_test.cpp` | `SBC A, n` |
| `cp_r_test.cpp` | `CP A, r` for register operands B, C, D, E, H, and L |
| `cp_a_from_indirect_hl_test.cpp` | `CP A, (HL)` |
| `cp_a_test.cpp` | `CP A, A` |
| `cp_a_n8_test.cpp` | `CP A, n` |
| `and_r_test.cpp` | `AND A, r` for register operands B, C, D, E, H, and L |
| `and_a_from_indirect_hl_test.cpp` | `AND A, (HL)` |
| `and_a_test.cpp` | `AND A, A` |
| `and_a_n8_test.cpp` | `AND A, n` |
| `xor_r_test.cpp` | `XOR A, r` for register operands B, C, D, E, H, and L |
| `xor_a_from_indirect_hl_test.cpp` | `XOR A, (HL)` |
| `xor_a_test.cpp` | `XOR A, A` |
| `xor_a_n8_test.cpp` | `XOR A, n` |
| `or_r_test.cpp` | `OR A, r` for register operands B, C, D, E, H, and L |
| `or_a_from_indirect_hl_test.cpp` | `OR A, (HL)` |
| `or_a_test.cpp` | `OR A, A` |
| `or_a_n8_test.cpp` | `OR A, n` |
| `inc_r_test.cpp` | `INC r` |
| `inc_indirect_hl_test.cpp` | `INC (HL)` |
| `inc_rr_test.cpp` | `INC rr` for BC, DE, HL, and SP |
| `dec_r_test.cpp` | `DEC r` |
| `dec_indirect_hl_test.cpp` | `DEC (HL)` |
| `dec_rr_test.cpp` | `DEC rr` for BC, DE, HL, and SP |
| `cpl_test.cpp` | `CPL` accumulator complement and flags |
| `ccf_test.cpp` | `CCF` carry toggling, Zero preservation, and Subtract/HalfCarry clearing |
| `daa_test.cpp` | `DAA` decimal adjustment with Subtract clear and set |
| `scf_test.cpp` | `SCF` Carry setting, Zero preservation, and Subtract/HalfCarry clearing |
| `load_hl_indirect_increment_a_test.cpp` | `LD A, (HL+)` |
| `load_hl_indirect_decrement_a_test.cpp` | `LD A, (HL-)` |
| `load_indirect_8bit_test.cpp` | `LD (HL), n` |
| `jump_a16_test.cpp` | `JP a16` |
| `jump_conditional_a16_test.cpp` | `JP cc, a16` |
| `jump_hl_test.cpp` | `JP HL` |
| `ret_test.cpp` | `RET` |
| `ret_conditional_test.cpp` | `RET cc` taken and untaken behavior |
| `reti_test.cpp` | `RETI` return and interrupt-enable behavior |
| `rst_test.cpp` | `RST` return-address push and vector jump behavior |
| `jump_relative_e8_test.cpp` | `JR e8` |
| `jump_relative_conditional_e8_test.cpp` | `JR cc, e8` |
| `di_test.cpp` | `DI` immediate disable and cancellation of a pending `EI` enable |
| `ei_test.cpp` | `EI` delayed enable, including the following multi-cycle instruction |

### Extended Instruction Tests (`emulatorlib/test/src/extended_instructions_tests/`)

| File | Instruction(s) |
|------|----------------|
| `bit_test.cpp` | CB-prefixed `BIT b, r` for register targets, including bit-result flags and preserved Carry |
| `bit_indirect_hl_test.cpp` | CB-prefixed `BIT b, (HL)` read, cycle sequencing, and flags |
| `res_test.cpp` | CB-prefixed `RES b, r` for register targets; flags are preserved |
| `res_indirect_hl_test.cpp` | CB-prefixed `RES b, (HL)` read, write, cycle sequencing, and flags |
| `rlc_test.cpp` | CB-prefixed `RLC r` for register targets, including rotate result and flags |
| `rlc_indirect_hl_test.cpp` | CB-prefixed `RLC (HL)` read, write, cycle sequencing, and flags |
| `rl_test.cpp` | CB-prefixed `RL r` for register targets, including Carry and flags |
| `rl_indirect_hl_test.cpp` | CB-prefixed `RL (HL)` read, write, cycle sequencing, Carry input, and flags |
| `rr_test.cpp` | CB-prefixed `RR r` for register targets, including Carry and flags |
| `rr_indirect_hl_test.cpp` | CB-prefixed `RR (HL)` read, write, cycle sequencing, Carry input, and flags |
| `rrc_test.cpp` | CB-prefixed `RRC r` for register targets, including rotate result and flags |
| `rrc_indirect_hl_test.cpp` | CB-prefixed `RRC (HL)` read, write, cycle sequencing, and flags |
| `sla_test.cpp` | CB-prefixed `SLA r` for register targets, including shift result and flags |
| `sla_indirect_hl_test.cpp` | CB-prefixed `SLA (HL)` read, write, cycle sequencing, and flags |
| `sra_test.cpp` | CB-prefixed `SRA r` for register targets, including arithmetic shift result and flags |
| `sra_indirect_hl_test.cpp` | CB-prefixed `SRA (HL)` read, write, cycle sequencing, and flags |
| `swap_test.cpp` | CB-prefixed `SWAP r` for register targets, including nibble exchange and flags |
| `swap_indirect_hl_test.cpp` | CB-prefixed `SWAP (HL)` read, write, cycle sequencing, and flags |
| `srl_test.cpp` | CB-prefixed `SRL r` for register targets, including logical shift result and flags |
| `srl_indirect_hl_test.cpp` | CB-prefixed `SRL (HL)` read, write, cycle sequencing, and flags |
| `set_test.cpp` | CB-prefixed `SET b, r` for register targets; flags are preserved |
| `set_indirect_hl_test.cpp` | CB-prefixed `SET b, (HL)` read, write, cycle sequencing, and preserved flags |

### Test Conventions

- Put emulator tests in the `EmulatorLib::Test` namespace.
- Name fixtures and test classes in PascalCase with a `Test` suffix, such as `JumpA16Test`.
- Use `TEST_F` for fixture tests and `TEST_P` with `INSTANTIATE_TEST_SUITE_P` for parameterized instruction tests.
- Name tests after the behavior under test in PascalCase; use underscores only to separate a scenario or expected
  outcome when that improves readability.
- Derive parameterized instruction-test names from `OpCodeToInstructionName`; do not duplicate instruction names in test
  data.
- CB-prefixed instruction tests live in `extended_instructions_tests/` and use `ExtendedOpCodeToInstructionName` and
  the extended opcode lookup table in `instruction_translation.cpp` to name cases. The test program includes both the CB
  prefix and extended opcode byte; parameter data identifies the extended opcode.

### Instruction Test Pattern

Group an instruction or instruction family in one test suite. Parameterize the suite when multiple opcodes share
the same behavior, including the cycle-accuracy and flag tests.

Use an `ExecutingOpcode` test to verify the detailed instruction pseudocode and timing. Set up the program,
registers, and relevant bus expectations, then assert the relevant state after each `m_cpu.Step()` call: intermediate
registers and flags, program counter, instruction register, and bus activity as applicable. Include the final
completion and next-opcode fetch where the execution model calls for them.

Use separate tests in the same suite for individual flag scenarios and outcomes, for example
`ExecutingOpcode_ResultIsZero_ZeroFlagIsSet` and `ExecutingOpcode_ResultIsNotZero_ZeroFlagIsNotSet`. These tests
run the instruction to completion and assert the relevant result and flags; they do not repeat the cycle-by-cycle
assertions already covered by `ExecutingOpcode`. Cover flag preservation as well as setting and clearing where the
instruction requires it.

### Parameterized Instruction Test Names

For base opcode suites, derive every `INSTANTIATE_TEST_SUITE_P` parameter name from
`OpCodeToInstructionName(info.param.instruction)`. For CB-prefixed suites, use
`ExtendedOpCodeToInstructionName(info.param.instruction)`. Instruction names are defined only in
`instruction_translation.cpp`; do not duplicate a name in the test parameter data.

### Utility Tests (`utilitylib/test/`)

| File                       | Tests                                       |
|----------------------------|---------------------------------------------|
| `add_with_carry_test.cpp`  | AddWithCarry function                       |
| `sub_with_borrow_test.cpp` | SubWithBorrow and SubWithBorrowIn functions |
| `bit_mask_test.cpp`        | BitMask constexpr                           |

## Building & Running Tests

```bash
cmake --build --preset conan-debug
ctest --test-dir build/Debug --output-on-failure
```

To run one Google Test:

```bash
./build/Debug/emulatorlib/test/emulatorlibTest --gtest_filter='EiTest.*'
```
