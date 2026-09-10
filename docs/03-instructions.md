# Instruction Set Architecture

## Instruction Handler Pattern

All instructions are implemented as C++23 coroutines with signature:

```cpp
auto Execute<Name>(AddressBus& addressBus, CpuRegisters& cpuRegisters) -> InstructionHandler
```

### Multi-Cycle Timing

- `co_yield std::monostate{}` — yield control, indicating the bus is busy (1 M-cycle)
- `co_return` — instruction complete
- The `co_yield` between bus operations prevents reading the next instruction while the bus is active

### Bus Operations

- `addressBus.ReadFromAddress(address)` — read a byte from memory
- `addressBus.WriteToAddress(address, data)` — write a byte to memory
- `cpuRegisters.programCounter++` — increment PC for fetching operands
- `cpuRegisters.zRegister` / `cpuRegisters.wRegister` — temporary storage (used as scratch registers)

## Register Helpers (`instruction_handlers/register_io_helpers.h`)

Lambda-based register accessors used with the template pattern:

```cpp
using RegisterA = decltype([](CpuRegisters& registers) -> Register8Bit& { return registers.accumulator; });
using RegisterBC = decltype([](CpuRegisters& registers) -> Register16Bit& { return registers.bcRegister; });
```

All available: `RegisterA`, `RegisterB`, `RegisterC`, `RegisterD`, `RegisterE`, `RegisterH`, `RegisterL`,
`RegisterAF`, `RegisterBC`, `RegisterDE`, `RegisterHL`, `RegisterSP`

### 16-bit Modifiers

```cpp
using Register16BitIncrement = decltype([](Register16Bit& reg) { reg.value++; });
using Register16BitDecrement = decltype([](Register16Bit& reg) { reg.value--; });
using RegisterNoModifier = decltype([](Register16Bit& /*reg*/) {});
```

## Register Concepts (`instruction_handlers/register_concepts.h`)

```cpp
template <typename T> concept ReturnsRegister8Bit  // T(registers) -> Register8Bit&
template <typename T> concept ReturnsRegister16Bit // T(registers) -> Register16Bit&
template <typename T> concept RegisterModifier8Bit // t(reg) -> void
template <typename T> concept RegisterModifier16Bit // t(reg) -> void
template <typename T> concept RegisterModifier      // either 8-bit or 16-bit modifier
```

## Flag Helpers (`instruction_handlers/flag_helpers.h`)

- `CarryIn(registers)` reads the current Carry flag for instructions that consume it.
- `ApplyAdditionFlags(registers, result, carryPerBit)` rebuilds Zero, HalfCarry, and Carry after 8-bit addition,
  clearing Subtract.
- `LD HL, SP+e8` only shares `CarryIn`; it clears Zero and derives its flags according to its distinct instruction
  rules.
- Each handler header directly includes every public emulator header needed by symbols it uses; it must not depend on an
  incidental include order.

## Implemented Instructions

### Load/Move Instructions

| Opcode(s)                                | Mnemonic        | Handler                                            | Description                                |
|------------------------------------------|-----------------|----------------------------------------------------|--------------------------------------------|
| 0x00                                     | NOP             | `ExecuteNoop`                                      | No operation                               |
| 0x01, 0x11, 0x21, 0x31                   | `LD rr, nn`     | `ExecuteLoad*Rn16`                                 | Load 16-bit register with 16-bit immediate |
| 0x06-0x0E, 0x16-0x1E, 0x26-0x2E, 0x3E    | `LD r, n`       | `ExecuteLoad*rn8`                                  | Load 8-bit register with 8-bit immediate   |
| 0x40-0x7F (subset)                       | `LD r, r'`      | `ExecuteLoadRegReg`                                | Register-to-register transfer              |
| 0x70-0x77, 0x77                          | `LD (HL), r`    | `ExecuteLoadIndirectFrom`                          | Store register at HL address               |
| 0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x7E | `LD r, (HL)`    | `ExecuteLoadRIndirectRr`                           | Load from HL address into register         |
| 0x02, 0x12                               | `LD (BC/DE), A` | `ExecuteLoadIndirectFrom`                          | Store A at BC/DE address                   |
| 0x0A, 0x1A                               | `LD A, (BC/DE)` | `ExecuteLoadRIndirectRr`                           | Load from BC/DE into A                     |
| 0x22, 0x32                               | `LD (HL+/-), A` | `ExecuteLoadIndirectFrom` with increment/decrement | Store A, adjust HL                         |
| 0x2A, 0x3A                               | `LD A, (HL+/-)` | `ExecuteLoadRIndirectRr` with modifier             | Load from adjusted HL into A               |
| 0x08                                     | `LD (nn), SP`   | `ExecuteLoadA16SP`                                 | Store SP at 16-bit address                 |
| 0xE0                                     | `LD (a), A`     | `ExecuteLoadIndirectA8FromA`                       | Store A at 0xFF00 + a                      |
| 0xE2                                     | `LD (C), A`     | `ExecuteLoadIndirectCFromA`                        | Store A at 0xFF00 + C                      |
| 0xEA                                     | `LD (aaaa), A`  | `ExecuteLoadIndirectA16FromA`                      | Store A at 16-bit address                  |
| 0xF0                                     | `LD A, (a)`     | `ExecuteLoadAFromIndirectA8`                       | Load A from 0xFF00 + a                     |
| 0xF2                                     | `LD A, (C)`     | `ExecuteLoadAFromIndirectC`                        | Load A from 0xFF00 + C                     |
| 0xFA                                     | `LD A, (aaaa)`  | `ExecuteLoadAFromIndirectA16`                      | Load A from 16-bit address                 |
| 0xF8                                     | `LD HL, SP+e`   | `ExecuteLoadSPFromHlPlusE8`                        | HL = SP + sign-extended e8                 |
| 0xF9                                     | `LD SP, HL`     | `ExecuteLoadReg16Reg16`                            | SP = HL                                    |

### Push/Pop Instructions

| Opcode                 | Mnemonic  | Handler         | Description                     |
|------------------------|-----------|-----------------|---------------------------------|
| 0xC1, 0xD1, 0xE1, 0xF1 | `POP rr`  | `ExecutePopRr`  | Pop 16-bit register from stack  |
| 0xC5, 0xD5, 0xE5, 0xF5 | `PUSH rr` | `ExecutePushRr` | Push 16-bit register onto stack |

### Arithmetic Instructions

#### Addition

| Opcode(s) | Mnemonic   | Handler         | Description                   |
|-----------|------------|-----------------|-------------------------------|
| 0x80-0x87 | `ADD A, r` | `ExecuteAddR`   | A = A + r (r includes `(HL)`) |
| 0xC6      | `ADD A, n` | `ExecuteAddAn8` | A = A + n (immediate)         |

#### Addition with Carry

| Opcode(s) | Mnemonic   | Handler         | Description                            |
|-----------|------------|-----------------|----------------------------------------|
| 0x88-0x8F | `ADC A, r` | `ExecuteAdcR`   | A = A + r + C flag (r includes `(HL)`) |
| 0xCE      | `ADC A, n` | `ExecuteAdcAn8` | A = A + n + C flag                     |

Note: Explicit `(HL)` variants are implemented and tested: `ADD A,(HL)` (0x86) via `ExecuteAddAFromIndirectHL`, and
`ADC A,(HL)` (0x8E) via `ExecuteAdcAFromIndirectHL`.

#### Increment/Decrement

| Opcode(s)                                | Mnemonic | Handler       | Description |
|------------------------------------------|----------|---------------|-------------|
| 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x3C | `INC r`  | `ExecuteIncR` | r++         |
| 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x3D | `DEC r`  | `ExecuteDecR` | r--         |

## Unimplemented Instructions

Major categories not yet implemented:

- **Control flow**: JP, JR, CALL, RET, RST, DJNZ, STOP
- **Logic**: AND, OR, XOR, CPL
- **Shift/Rotate**: SLA, SRL, SLL, SLL, RL, RR, RLC, RRC
- **Decimal adjustment**: DAA
- **Special**: SCF, CCF, HALT, EI, DI
- **CB-prefixed**: All CB-prefixed instructions (bit operations, shifts, rotates)

## Opcode Reference Table

See `instruction_translation.cpp` `CreateOpCodeLut()` for the complete mapping of implemented opcodes.
