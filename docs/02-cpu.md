# CPU Architecture

## CPU Class (`emulatorlib/include/emulatorlib/cpu.h`, `emulatorlib/src/cpu.cpp`)

The CPU is the central execution unit. It owns registers and dispatches opcodes via a lookup table.

### Construction
- Takes a reference to `AddressBus`
- Initializes registers to boot-state values (per [gbdev power-up docs](https://github.com/gbdev/pandocs/blob/master/src/Power_Up_Sequence.md)):
  - `AF` = `0x0100` (A=0x01, flags masked to Zero)
  - `BC` = `0x0013`
  - `DE` = `0x00D8`
  - `HL` = `0x014D`
  - `WZ` = `0x0000` (internal register for indirect addressing)
  - `PC` = `0x0100` (start address)
  - `SP` = `0xFFFE`
  - `IFF` / `IE` = unimplemented
  - Interrupts, timing, and sound not yet implemented

### Execution Model
The CPU runs on a single public method: `Step()`

```
Step() {
    1. Resume current coroutine (co_yield or co_return)
    2. If coroutine not done → return (multi-cycle instruction)
    3. Read next opcode from memory at PC
    4. Increment PC
    5. Fetch next handler from opcode table
    6. Start new coroutine
}
```

### Opcode Dispatch
- `std::array<std::function<InstructionHandler(...)>, 256>` mapping opcode → coroutine factory
- Created by `CreateOpcodeHandlers()` in `opcode_handler_map.h`
- Unimplemented opcodes throw `std::runtime_error("Unimplemented opcode")`
- 0x00 (NOP) is the only explicitly trivial handler

### Coroutine-Based Multi-Cycle Execution
- Each instruction is a coroutine (`InstructionHandler`)
- `co_yield std::monostate{}` = "wait one M-cycle" (bus is busy)
- `co_return` = instruction complete, can fetch next opcode
- Bus operations (read/write) must yield between them to respect timing
- Exception safety: coroutine stores `std::exception_ptr`, re-thrown in next `Step()`

## CpuRegisters (`emulatorlib/include/emulatorlib/cpu_registers.h`)

All CPU registers. Uses a layered design with backing storage and `Register<T>` wrappers.

### Register Pairs (16-bit)
| Register | Backing        | Purpose          |
|----------|---------------|------------------|
| `af`     | `backingAf`   | A + Flags        |
| `bc`     | `backingBc`   | General purpose  |
| `de`     | `backingDe`   | General purpose  |
| `hl`     | `backingHl`   | Memory addressing|
| `wz`     | `backingWz`   | Internal (indirect) |
| `PC`     | `backingPC`   | Program counter  |
| `SP`     | `backingSP`   | Stack pointer    |

### Register Bytes (8-bit)
- Each 16-bit register exposes `.UpperByteAsRegister()` and `.LowerByteAsRegister()` 
- These return `Register8Bit` references that share the same backing memory
- E.g., `accumulator` references the upper byte of `afRegister`, `flags` references the lower byte

### Special Registers
- `instructionRegister` (B) — holds current opcode being executed
- `interruptEnableRegister` (IFF/IE) — currently unused but present

## CpuFlags (`emulatorlib/include/emulatorlib/cpu_flags.h`, `emulatorlib/src/cpu_flags.cpp`)

Four CPU flags stored in bits 7, 6, 5, 4 of the flags register (bits 3-0 are always 1):

| Flag        | Bit | Value  | Description                    |
|-------------|-----|--------|--------------------------------|
| Zero (Z)    | 7   | `0x80` | Result was zero                |
| Subtract (N)| 6   | `0x40` | Previous operation was SUB/SBC |
| Half Carry (H)| 5 | `0x20` | Carry/borrow from bit 3        |
| Carry (C)   | 4   | `0x10` | Carry out from bit 7           |

### Usage Pattern
Flags are combined via `operator|` and set as bytes:
```cpp
cpuRegisters.flags.value = 0x00_b;
cpuRegisters.flags.value |= result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b;
cpuRegisters.flags.value |= (carry & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b;
cpuRegisters.flags.value |= (carry & UtilityLib::BitMask<7>) > 0x00_b ? CpuFlags::Carry.AsByte() : 0x00_b;
```

## Register Class (`emulatorlib/include/emulatorlib/register.h`)

A template wrapper `Register<T>` that enforces a mask on all operations:
- `value` — reference to backing storage
- `mask` — bits that are writable/readable
- Assignment: `value = (t & mask)` — only masked bits are modified
- Addition: masked arithmetic without carry-through
- Conversion to `T`: returns `value & mask`
- `LowerByteAsRegister()` / `UpperByteAsRegister()` — expose byte sub-registers (only for `Register16Bit`)
- Supports `operator<<=>` for comparison
