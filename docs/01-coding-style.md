# Coding Style

## C++ Standard

**C++26.** All C++26 features are available and should be used when appropriate. Do not write C++11, C++14, or C++17 code when a modern C++26 alternative exists.

### Formatting and Output

**Use `std::format` and `std::println`. Never use `std::stringstream`, `std::cout`, or `strstream`.**

```cpp
// Preferred: modern and type-safe
std::println("Register AF = 0x{:04X}", cpuRegisters.afRegister.value);
std::format("Cartridge title: {}", cartridge.Title());

// Avoid: legacy, error-prone, heap allocations
std::stringstream ss;
ss << "Register AF = 0x" << std::hex << std::setw(4) << std::setfill('0')
   << static_cast<int>(cpuRegisters.afRegister.value);
```

Use `std::println` for log/debug output and `std::format` when you need the string as a value. Use `std::println` instead of `std::cout`, and do not use `<sstream>` for application formatting.

### Ranges

**Use `std::ranges` whenever you need to filter, transform, or iterate over a container or view.**

```cpp
// Preferred: declarative, lazy, composable
std::ranges::find_if(instructions, [](const auto& inst) {
    return inst.name == "ADD A, (HL)";
});

auto titles = cartridge_data | std::views::filter([](auto const& entry) {
    return entry.valid;
}) | std::views::transform([](auto const& entry) {
    return entry.title;
});

// Avoid: imperative, eager, verbose
for (size_t i = 0; i < instructions.size(); ++i) {
    if (instructions[i].name == "ADD A, (HL)") {
        return &instructions[i];
    }
}
```

Use `std::ranges::find_if`, `std::ranges::for_each`, `std::views::filter`, `std::views::transform`, `std::views::take`, etc. Prefer range-based algorithms over manual loops.

### Layout

Use four spaces for indentation and place opening braces on the line following a declaration or control statement. Do not rely on an editor's default formatting; preserve these conventions until a project formatter is configured.

```cpp
class Cartridge
{
public:
    explicit Cartridge(std::filesystem::path romPath);
};

if (romSize < minSize)
{
    throw InvalidRomException(romPath);
}
```

Prefer trailing return types for nontrivial function declarations and definitions:

```cpp
[[nodiscard]] auto ReadFromAddress(uint16_t address) const noexcept -> std::byte;
```

### Concepts

**Always constrain template parameters with concepts. Never use SFINAE with `std::enable_if` or unconstrained `auto` in templates.**

```cpp
// Preferred: explicit, readable, great error messages
template <ReturnsRegister8Bit RegisterAccessor>
constexpr auto ExecuteAddA(
    AddressBus& addressBus, CpuRegisters& cpuRegisters
) noexcept -> InstructionHandler { /* ... */ }

// Avoid: no constraints, terrible error messages on misuse
template <typename T>
auto ExecuteSomething(T&& thing) { /* ... */ }

// Avoid: SFINAE noise
template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
auto process(T value) { /* ... */ }
```

Use concepts for all template parameters. Define project-specific concepts in the relevant header (see `register_concepts.h` for examples). Use standard library concepts (`std::integral`, `std::ranges::range`, `std::copyable`, etc.) when they apply.

## Casts

**Never use C-style casts.** Use the typed alternatives, which are checked, explicit, and easy to search for.

| Instead of | Use |
|-----------|-----|
| `(T)expr` for numeric conversions | `static_cast<T>(expr)` |
| `(T)ptr` / `(void*)ptr` pointer casts | `static_cast` (up/downcast) or `reinterpret_cast` (bit-level reinterpreting only) |
| `(const T)expr` removing const | `const_cast<T>(expr)` (avoid; const-correct design preferred) |
| `(T)expr` combining const removal + reinterpret | Separate `const_cast` + `reinterpret_cast` (avoid where possible) |

For reinterpreting object representation between related types (e.g., `uint32_t` ↔ `float`, or extracting sub-objects from aligned storage), prefer `std::bit_cast` over `reinterpret_cast` — it is `constexpr`, well-defined, and never aliases.

## Ownership and Aliasing

**Use references over raw pointers for function parameters and non-owning access. Use `std::unique_ptr` over `std::shared_ptr`.**

- Function parameters: `auto Foo(std::string_view name, AddressBus& bus)` — never `char*` or `AddressBus*` when a reference or `std::string_view` expresses the same non-null contract. Raw pointers are allowed only when a nullable pointer is genuinely part of the API, and prefer `std::optional` for that too.
- Ownership: `std::unique_ptr` is the default for dynamically owned objects. Only reach for `std::shared_ptr` when ownership is genuinely shared across multiple owners with independent lifetimes — and say so in a comment. Prefer value semantics and RAII over both.
- Non-owning pointers (e.g., `std::coroutine_handle::promise()`, observer pointers) are fine, but document that they do not own the pointee.

## Error Handling

**Throw `std::runtime_error` or custom exception types for unrecoverable errors. Use `[[nodiscard]]` MathematicalResult-style types for operations that may fail but the caller should handle explicitly.**

```cpp
// Preferred: unrecoverable error
if (romSize < minSize) {
    throw InvalidRomException("ROM too small: " + std::to_string(romSize));
}

// Preferred: recoverable operation
auto result = AddWithCarry(a, b);
if (result.carryBits == 0x80_b) {
    // handle carry
}

// Avoid: discarding a result that should be checked
[[nodiscard]] auto ParseConfig(std::string_view path);
ParseConfig("config.ini");  // compiler warning: ignored
```

## Include Order

**Order: header corresponding to the source file → local project headers → third-party headers → standard library headers. Leave one blank line between each section.**

```cpp
// File: emulatorlib/src/cpu.cpp
#include "emulatorlib/cpu.h"          // corresponding header (1)

#include "emulatorlib/cpu_registers.h"  // local project (2)
#include "emulatorlib/cpu_flags.h"
#include "emulatorlib/address_bus.h"

#include <gtest/gtest.h>                // third-party (3)
#include <gmock/gmock.h>

#include <array>                         // std library (4)
#include <coroutine>
#include <functional>
#include <vector>
```

## Attributes and Specifiers

**Use `[[nodiscard]]`, `constexpr`, `const`, and `noexcept` whenever they are valid.**

- `[[nodiscard]]` on functions whose return value the caller should not ignore (especially status/error/result types)
- `constexpr` for all functions that can be evaluated at compile time
- `const` on member functions that do not modify the object state (and on object references that are not modified)
- `noexcept` on functions that do not throw (including most getters, destructors, move operations, and bus I/O methods)

```cpp
// Preferred
[[nodiscard]] auto ReadFromAddress(uint16_t address) const noexcept -> std::byte;
[[nodiscard]] constexpr auto Title() const -> std::string_view;
void WriteToAddress(uint16_t address, std::byte data) noexcept;
[[nodiscard]] auto HasValidNintendoLogo() const -> bool;

// Avoid
std::byte ReadFromAddress(uint16_t address) { /* ... */ }  // missing noexcept, const
auto Title() -> std::string_view { /* ... */ }  // missing constexpr, const, [[nodiscard]]
```

## Naming Conventions

| Kind | Convention | Example |
|------|-----------|---------|
| **Functions** | `PascalCase` | `ReadFromAddress`, `ExecuteAddA` |
| **Classes / Structs** | `PascalCase` | `CpuRegisters`, `Cartridge` |
| **Enums** | `PascalCase` | `CartridgeType` |
| **Enum values** | `SCREAMING_SNAKE_CASE` | `ROM_ONLY`, `MBC1` |
| **Variables (local)** | `camelCase` | `address`, `data`, `result` |
| **Member variables** | `m_camelCase` | `m_addressableMock`, `m_backingHl` |
| **Global variables** | `g_camelCase` | `g_logger` |
| **Namespaces** | `PascalCase` | `EmulatorLib`, `UtilityLib` |
| **Files / Paths** | `snake_case` | `cpu_registers.h`, `add_with_carry.h` |

**No `p` prefix for pointers.** Use `m_` prefix for member variables and `g_` prefix for globals.

## Special Member Functions

**Prefer zero user-declared special member functions. If a type manages a resource and declares any of destructor, copy/move constructor, or copy/move assignment operator, declare or delete all five (Rule of 5).**

```cpp
// Preferred: rule of zero, no custom special members
class Cartridge {
public:
    explicit Cartridge(std::filesystem::path romPath);
    [[nodiscard]] auto Title() const -> std::string_view;
    // ...
};

// Preferred: rule of five, all declared
class NonCopyableMovable {
public:
    NonCopyableMovable();
    ~NonCopyableMovable();
    NonCopyableMovable(NonCopyableMovable&&) noexcept;
    auto operator=(NonCopyableMovable&&) noexcept -> NonCopyableMovable&;
    NonCopyableMovable(const NonCopyableMovable&) = delete;
    auto operator=(const NonCopyableMovable&) -> NonCopyableMovable& = delete;
};

// Avoid: a user-declared destructor suppresses implicit moves
class BadPartiallyDefined {
    ~BadPartiallyDefined();
};
```

Prefer `std::unique_ptr`, `std::shared_ptr`, and value semantics. Delete (don't just omit) copy operations when a type should be non-copyable.

## Comments and Documentation

**The code is self-documenting. Comments must only be added when the intent is not clear from reading the code alone.**

Comments must explain the **why** and the **how**, never the **what**. The what is already in the code — repeating it in a comment is noise.

Do not use emoji characters in documentation or source code.

```cpp
// Preferred: explains why, not what
auto result = AddWithCarry(a, b);  // use AddWithCarry (not AddWithCarryIn) because the carry flag
                                   // will be checked by the instruction handler after this point

// Avoid: redundant, says what the code already says
auto result = AddWithCarry(a, b);  // add a and b

// Preferred: explains non-obvious intent
// The Game Boy delays the PC increment for LD (nn), SP
// so we must increment PC before reading the operand
++cpuRegisters.programCounter;
```

## Function Design

**Functions must do one thing. The function name must describe that one thing exactly.**

A function name like `DoSomethingAndSomething` is doing too much and must be split. If you need the word "and" in a function name, it is doing more than one thing.

```cpp
// Preferred: single responsibility, name describes the action
[[nodiscard]] auto ValidateRomHeader(const RomData& data) -> bool;
void ApplyCarryFlags(CpuRegisters& registers, std::byte carryBits);
std::byte ReadOperandFromAddress(const AddressBus& bus, uint16_t address);

// Avoid: multiple responsibilities, "and" in the name
void LoadAndProcessRom(const std::filesystem::path& path);
auto ExecuteAddAFromIndirectHlOrRegister(...);
void ParseHeaderAndValidateChecksumAndStoreTitle();
```

Rule of thumb: **if a function name contains "and" or "or", it likely violates single responsibility.** Split it. Prefer small, composable functions that each do one thing and have a name that makes their purpose obvious without needing to read their body.
