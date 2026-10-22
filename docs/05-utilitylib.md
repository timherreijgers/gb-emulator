# Utility Library

## Overview

Utility library providing low-level byte operations for the emulator.

`UtilityLib` is exposed as the `utilitylib/src/utilitylib.cppm` named module interface and is consumed with
`import utilitylib;`. The former `utilitylib/include/utilitylib/` headers and their public include path were removed
in the module migration. This is a breaking API change: include-based consumers must migrate to `import utilitylib;`
and link against the `utilitylib` CMake target.

Module dependencies are not automatically re-exported by other modules. Import `utilitylib` directly in a module
partition that uses its declarations or the `std::byte` formatter, even if another partition imports it.

## Integral Suffixes (`src/integral_suffixes.cppm`)

User-defined literals for explicit type-safe byte/integer literals:

```cpp
0x00_b    // std::byte
0xFF_u8   // uint8_t
0xFFFF_u16 // uint16_t
0x7F_s8   // int8_t
0x7FFF_s16 // int16_t
// ... also _u32, _s32, _u64, _s64
```

## Byte Utilities (`src/byte_utils.cppm`)

Overloads for `+` and `-` on `std::byte`:

```cpp
operator+(std::byte, std::byte) -> std::byte
operator+(std::byte, integral) -> std::byte
operator-(std::byte, std::byte) -> std::byte
operator-(std::byte, integral) -> std::byte
```

## Numeric Limits (`src/byte_utils.cppm`)

`UtilityLib::numeric_limits<T>` inherits from `std::numeric_limits<T>` and specializes its own `std::byte` case by
inheriting the limits for `unsigned char`.

## Mathematical Result (`src/mathematical.cppm`)

Common result struct for mathematical operations:

```cpp
namespace UtilityLib {

template <typename T>
concept IntegralOrByte = std::unsigned_integral<T> || std::is_same_v<T, std::byte>;

template <IntegralOrByte T>
struct [[nodiscard]] MathematicalResult {
    T result;
    T carryBits;
};
}
```

## Add With Carry (`src/mathematical.cppm`)

```cpp
namespace UtilityLib {

constexpr auto AddWithCarry = [](IntegralOrByte auto left, IntegralOrByte auto right) noexcept -> MathematicalResult<decltype(left)>;

template <IntegralOrByte T>
[[nodiscard]] constexpr auto AddWithCarryIn(T left, T right, bool carryIn) noexcept -> MathematicalResult<T>;
}
```

- `AddWithCarry` — computes `a + b` using bitwise logic to derive carry bits without conditional branches
- `AddWithCarryIn` — adds an explicit one-bit carry-in to two operands (delegates to `AddWithCarry`)
- Accepts only unsigned integral types or `std::byte` (via `IntegralOrByte` concept)
- Carry bits use bitmask positions (bit 3 for half-carry, bit 7 for carry)
- The `IntegralOrByte` concept is defined in `src/mathematical.cppm`

## Sub With Borrow (`src/mathematical.cppm`)

```cpp
namespace UtilityLib {

constexpr auto SubWithBorrow = [](IntegralOrByte auto left, IntegralOrByte auto right) constexpr noexcept -> MathematicalResult<decltype(left)>;

constexpr auto SubWithBorrowIn = [](IntegralOrByte auto left, IntegralOrByte auto right, bool borrowIn) constexpr noexcept -> MathematicalResult<decltype(left)>;
}
```

- Implemented using Borrow-Lookahead subtractor algorithm
- `SubWithBorrowIn` subtracts an explicit one-bit borrow-in and combines the borrow masks from both subtraction steps
- Returns a `MathematicalResult<T>` (same struct as `AddWithCarry`) with `result` and `carryBits`
- Uses the `IntegralOrByte` concept from `src/mathematical.cppm`
- Used for `SUB`/`SBC` flag computation

## Bit Mask (`src/bit_mask.cppm`)

Compile-time bit mask constants:

```cpp
namespace UtilityLib {
template <uint8_t mask>
    requires ValidBitMask<mask>  // mask must be 0-7
constexpr auto BitMask = static_cast<std::byte>(1 << mask);
}
```

Used to extract specific carry/borrow bit positions:

- `BitMask<3>` = `0x08_b` (half-carry position)
- `BitMask<7>` = `0x80_b` (carry position)
