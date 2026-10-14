# CPU Architecture

## CPU Class (`emulatorlib/include/emulatorlib/cpu.h`, `emulatorlib/src/cpu.cpp`)

The CPU is the central execution unit. It owns registers and dispatches opcodes via a lookup table.

### Construction
- Takes a reference to `AddressBus`
- Initializes a simplified starting state; it does not execute the Nintendo boot ROM and is not a model-accurate hardware power-up sequence:
  - `AF` = `0x0180` (A=0x01, Zero flag set; the lower four flag bits are masked to zero)
  - `BC` = `0x0013`
  - `DE` = `0x00D8`
  - `HL` = `0x014D`
  - `WZ` = `0x0000` (internal register for indirect addressing)
  - `PC` = `0x0100` (start address)
  - `SP` = `0xFFFE`
  - Master interrupt state = `DISABLED`
  - The register values are selected by this implementation; actual post-boot values vary by Game Boy model
  - Interrupt servicing, memory-mapped interrupt registers, and sound are not yet implemented

### Execution Model
The CPU runs on a single public method: `Step()`

```
Step() {
    1. Resume current coroutine (co_yield or co_return)
    2. If coroutine not done → return (multi-cycle instruction)
    3. Promote a pending master-interrupt enable after the instruction following EI completes
    4. Read next opcode from memory at PC
    5. Increment PC
    6. Fetch next handler from opcode table
    7. Start new coroutine
}
```

### Opcode Dispatch
- `std::array<OpcodeHandler, 256>` mapping each opcode to its mnemonic and coroutine factory
- Created by `CreateOpcodeHandlers()` in `opcode_handler_map.h`
- Every opcode entry carries its mnemonic; unimplemented opcodes dispatch to `UnimplementedOpcode`, while illegal opcodes dispatch to `IllegalOpcode`
- Illegal opcodes are `0xD3`, `0xDB`, `0xDD`, `0xE3`, `0xE4`, `0xEB`, `0xEC`, `0xED`, `0xF4`, `0xFC`, and `0xFD`
- Both error messages include the current opcode value: `Unimplemented opcode: <value>` or `Illegal opcode: <value>`
- 0x00 (NOP) is the only explicitly trivial handler

### Coroutine-Based Multi-Cycle Execution
- Each instruction is a coroutine (`InstructionHandler`)
- `co_yield std::monostate{}` = advance one emulated M-cycle before the next coroutine resume
- `co_return` = instruction complete, can fetch next opcode
- The coroutine scheduler delays opcode fetching until the current handler completes; this is a simplified timing model, not bus arbitration or a complete hardware bus model
- Exception safety: coroutine stores `std::exception_ptr`, re-thrown in next `Step()`

## CpuRegisters (`emulatorlib/include/emulatorlib/cpu_registers.h`)

All CPU registers. Uses a layered design with backing storage and `Register<T>` wrappers.

### Register Pairs (16-bit)
| Register | Backing        | Purpose          |
|----------|---------------|------------------|
| `afRegister` | `backingAfRegister` | A + Flags |
| `bcRegister` | `backingBcRegister` | General purpose |
| `deRegister` | `backingDeRegister` | General purpose |
| `hlRegister` | `backingHlRegister` | Memory addressing |
| `wzRegister` | `backingWzRegister` | Internal scratch pair; not an LR35902-visible register |
| `programCounter` | `backingProgramCounterRegister` | Program counter |
| `stackPointer` | `backingStackPointerRegister` | Stack pointer |

### Register Bytes (8-bit)
- Each 16-bit register exposes `.UpperByteAsRegister()` and `.LowerByteAsRegister()` 
- These return `Register8Bit` references that share the same backing memory
- E.g., `accumulator` references the upper byte of `afRegister`, `flags` references the lower byte

### Special Registers
- `instructionRegister` — internal latch for the current opcode; it is not the hardware `B` register
- `interruptEnableRegister` — unused storage only; it is not the memory-mapped Game Boy `IE` register (and `IFF` is not LR35902 terminology)
- `masterInterruptState` — master interrupt-enable state:
  - `DISABLED` — interrupt handling is disabled (the initial state)
  - `PENDING` — `EI` has requested enabling after its required delay
  - `ENABLED` — interrupt handling is enabled

`EI` first sets the state to `PENDING`. `Cpu::Step()` promotes it to `ENABLED` after the complete subsequent instruction,
including all emulated cycles of a multi-cycle instruction. This models the EI delay only; interrupt requests and
interrupt servicing are not implemented.
`DI` immediately sets the state to `DISABLED`, cancelling a pending `EI` enable.

## CpuFlags (`emulatorlib/include/emulatorlib/cpu_flags.h`, `emulatorlib/src/cpu_flags.cpp`)

Four CPU flags are stored in bits 7, 6, 5, and 4 of `F`. Bits 3-0 are unused and read as zero; `CpuRegisters` masks
them off when writing the `AF` pair:

| Flag        | Bit | Value  | Description                    |
|-------------|-----|--------|--------------------------------|
| Zero (Z)    | 7   | `0x80` | Result was zero                |
| Subtract (N)| 6   | `0x40` | Subtraction flag                |
| Half Carry (H)| 5 | `0x20` | Carry/borrow between bits 3 and 4 for 8-bit arithmetic |
| Carry (C)   | 4   | `0x10` | Carry/borrow out of bit 7 for 8-bit arithmetic |

### Usage Pattern
Flags are combined via `operator|` and set as bytes:
```cpp
cpuRegisters.flags.value = 0x00_b;
cpuRegisters.flags.value |= result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b;
cpuRegisters.flags.value |= (carry & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b;
cpuRegisters.flags.value |= (carry & UtilityLib::BitMask<7>) > 0x00_b ? CpuFlags::Carry.AsByte() : 0x00_b;
```

## Register Class (`emulatorlib/include/emulatorlib/register.h`)

A template wrapper `Register<T>` that applies a mask to selected operations:
- `value` — reference to backing storage
- `mask` — bits that are writable/readable
- Assignment, `+=`, `&=`, and `|=` apply the mask; `+` returns masked arithmetic without carry-through
- Conversion to `T` returns `value & mask`
- `LowerByteAsRegister()` / `UpperByteAsRegister()` — expose byte sub-registers (only for `Register16Bit`)
- Post-increment/decrement and comparisons operate directly on backing values
