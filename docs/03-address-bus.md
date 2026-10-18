# Address Bus & Devices

## AddressBus (`emulatorlib/src/address_bus/address_bus.cppm`, `emulatorlib/src/address_bus/address_bus.cpp`)

The address bus connects the CPU to the registered `AddressBusAddressable` objects. The current project connects a
cartridge and work RAM; it does not yet model the complete Game Boy address map or memory-mapped I/O.

Import `emulatorlib.address_bus` and link `emulatorlib` to use the bus and its addressable interface.

### Design

- Constructor takes `std::vector<std::reference_wrapper<AddressBusAddressable>> devices`
- Multiple devices can respond to the same address
- **Write**: broadcasts to ALL devices
- **Read**: OR-combines results from all devices (`data |= device.ReadFromAddress(address)`)

This is an implementation simplification, not Game Boy bus decoding or open-bus behavior. Each device is responsible
for ignoring addresses it does not handle.

### Methods

- `void WriteToAddress(uint16_t address, std::byte data) noexcept` — write to all devices
- `auto ReadFromAddress(uint16_t address) const noexcept -> std::byte` — OR-combine from all devices

## AddressBusAddressable (`emulatorlib/src/address_bus/address_bus.cppm`)

Pure virtual interface that all devices must implement:

```cpp
virtual void WriteToAddress(uint16_t address, std::byte data) noexcept = 0;
virtual auto ReadFromAddress(uint16_t address) const noexcept -> std::byte = 0;
```

## Cartridge (`emulatorlib/src/cartridge/cartridge.cppm`, `emulatorlib/src/cartridge/cartridge.cpp`)

Loads and exposes ROM data. ROM-size metadata is checked during construction; the separate logo and checksum methods
report validation results but do not reject the ROM.

Import `emulatorlib.cartridge` for its public types; consumers using `AddressBusAddressable` directly also import
`emulatorlib.address_bus`.

### Constructor

- Takes `std::filesystem::path romPath`
- Validates file size (0x150 ≤ size ≤ 0x800000)
- Requires an ordinary ROM-size code at `0x0148` no greater than 8 and checks that its computed size matches the file
- Does not validate the Nintendo logo or header checksum in the constructor

### Exceptions

- `InvalidRomException` — thrown for invalid ROM files (too small, bad size byte, etc.)
- `MismatchedRomSizeException` — header says one size, file is another

### Public Methods

| Method                   | Returns               | Description                                                              |
|--------------------------|-----------------------|--------------------------------------------------------------------------|
| `HasValidNintendoLogo()` | `bool`                | Checks 48-byte logo at 0x104                                             |
| `Title()`                | `string_view`         | Raw 16-byte view beginning at `0x0134`; no model-dependent title parsing |
| `CartridgeType()`        | `CartridgeType`       | Enum value read from the cartridge-type byte at `0x0147`                 |
| `RomSize()`              | `size_t`              | Computed ROM size from header byte at 0x148                              |
| `ValidHeaderChecksum()`  | `bool`                | Validates checksum at 0x14D                                              |
| `RomData()`              | `const vector<byte>&` | Raw ROM data                                                             |
| `RomPath()`              | `const path&`         | Original file path                                                       |

### ROM Header Offsets

| Offset          | Field                                                              |
|-----------------|--------------------------------------------------------------------|
| `0x0100`        | Entry point area (not necessarily a single reset-vector opcode)    |
| `0x0104`        | Nintendo logo (48 bytes; checked only by `HasValidNintendoLogo()`) |
| `0x0134–0x0143` | Legacy title bytes returned by `Title()`                           |
| `0x0146`        | SGB support flag                                                   |
| `0x0147`        | Cartridge type                                                     |
| `0x0148`        | ROM size code                                                      |
| `0x014D`        | Header checksum byte; checked only by `ValidHeaderChecksum()`      |

The enum recognizes the cartridge-type values listed in `cartridge.cppm`; it does not implement their mapper behavior.
`Cartridge::ReadFromAddress()` exposes only the first 32 KiB of ROM directly, and writes are ignored. There is no
bank switching, cartridge RAM, or special handling for nonstandard ROM-size codes.

## Ram (`emulatorlib/src/ram/ram.cppm`, `emulatorlib/src/ram/ram.cpp`)

Provides a simplified fixed 8 KiB work-RAM region without banking or the Game Boy's echo-RAM mapping.

- Address range: `0xC000–0xDFFF`
- Initial contents: zero
- Reads and writes outside the range are ignored; reads return `0x00`
- Connected to the address bus by `main.cpp`
