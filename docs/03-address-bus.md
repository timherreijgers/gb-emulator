# Address Bus & Devices

## AddressBus (`emulatorlib/include/emulatorlib/address_bus.h`, `emulatorlib/src/address_bus.cpp`)

The address bus connects the CPU to all addressable devices (ROM, RAM, I/O, etc.).

### Design
- Constructor takes `std::vector<std::reference_wrapper<AddressBusAddressable>> devices`
- Multiple devices can respond to the same address
- **Write**: broadcasts to ALL devices
- **Read**: OR-combines results from all devices (`data |= device.ReadFromAddress(address)`)

### Methods
- `void WriteToAddress(uint16_t address, std::byte data) noexcept` — write to all devices
- `auto ReadFromAddress(uint16_t address) const noexcept -> std::byte` — OR-combine from all devices

## AddressBusAddressable (`emulatorlib/include/emulatorlib/address_bus_addressable.h`)

Pure virtual interface that all devices must implement:
```cpp
virtual void WriteToAddress(uint16_t address, std::byte data) noexcept = 0;
virtual auto ReadFromAddress(uint16_t address) const noexcept -> std::byte = 0;
```

## Cartridge (`emulatorlib/include/emulatorlib/cartridge.h`, `emulatorlib/src/cartridge.cpp`)

Loads and exposes ROM data with header validation.

### Constructor
- Takes `std::filesystem::path romPath`
- Validates file size (0x150 ≤ size ≤ 0x800000)
- Validates Nintendo logo at offset 0x104 (48 bytes)
- Validates ROM size header byte at 0x148 (must be ≤ 8)
- Verifies calculated ROM size matches actual file size
- Validates header checksum at 0x134–0x14D

### Exceptions
- `InvalidRomException` — thrown for invalid ROM files (too small, bad size byte, etc.)
- `MismatchedRomSizeException` — header says one size, file is another

### Public Methods
| Method | Returns | Description |
|--------|---------|-------------|
| `HasValidNintendoLogo()` | `bool` | Checks 48-byte logo at 0x104 |
| `Title()` | `string_view` | 16-char title at offset 0x134 |
| `CartridgeType()` | `CartridgeType` | MBC type at offset 0x147 |
| `RomSize()` | `size_t` | Computed ROM size from header byte at 0x148 |
| `ValidHeaderChecksum()` | `bool` | Validates checksum at 0x14D |
| `RomData()` | `const vector<byte>&` | Raw ROM data |
| `RomPath()` | `const path&` | Original file path |

### ROM Header Offsets
| Offset | Field |
|--------|-------|
| 0x0100 | Reset vector (first opcode) |
| 0x0104 | Nintendo logo (48 bytes) |
| 0x0134 | Title (16 bytes) |
| 0x0146 | Cartridge type (byte) |
| 0x0147 | Cartridge type (extended) |
| 0x0148 | ROM size (power-of-2, ≤ 8) |
| 0x014D | Header checksum |

### Cartridge Types (enum)
ROM_ONLY (0x00), MBC1/1_RAM/1_RAM_BATTERY (0x01-0x03), MBC2/2_BATTERY (0x05-0x06), 
ROM_RAM/ROM_RAM_BATTERY (0x08-0x09), MMM01 variants (0x0B-0x0D), MBC3 variants (0x0F-0x13), 
MBC5 variants (0x19-0x1E), MBC6 (0x20), MBC7 variant (0x22), 
POCKET_CAMERA (0xFC), BANDAI_TAMA5 (0xFD), HUC3 (0xFE), HUC1_RAM_BATTERY (0xFF)
