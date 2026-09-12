# Utility Library

## Overview
Header-only utility library (`utilitylib/include/utilitylib/`) providing low-level byte operations for the emulator.

## Integral Suffixes (`integral_suffixes.h`)
User-defined literals for explicit type-safe byte/integer literals:
```cpp
0x00_b    // std::byte
0xFF_u8   // uint8_t
0xFFFF_u16 // uint16_t
0x7F_s8   // int8_t
0x7FFF_s16 // int16_t
// ... also _u32, _s32, _u64, _s64
```

## Byte Utilities (`byte_utils.h`)
Overloads for `+` and `-` on `std::byte`:
```cpp
operator+(std::byte, std::byte) -> std::byte
operator+(std::byte, integral) -> std::byte
operator-(std::byte, std::byte) -> std::byte
operator-(std::byte, integral) -> std::byte
```

## Numeric Limits (`numeric_limits.h`)
Custom `UtilityLib::numeric_limits<T>` specializing `std::numeric_limits<std::byte>` to use `unsigned char` limits (since `std::byte` lacks numeric limits in C++23).

## Mathematical Result (`mathematical_result.h`)

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

## Add With Carry (`add_with_carry.h`)
```cpp
namespace UtilityLib {

constexpr auto AddWithCarry = [](IntegralOrByte auto left, IntegralOrByte auto right) noexcept -> MathematicalResult<decltype(left)>;

[[nodiscard]] constexpr auto AddWithCarryIn(T left, T right, bool carryIn) noexcept -> MathematicalResult<T>;
}
```
- `AddWithCarry` — computes `a + b` using bitwise logic to derive carry bits without conditional branches
- `AddWithCarryIn` — adds an explicit one-bit carry-in to two operands (delegates to `AddWithCarry`)
- Accepts only unsigned integral types or `std::byte` (via `IntegralOrByte` concept)
- Carry bits use bitmask positions (bit 3 for half-carry, bit 7 for carry)
- The `IntegralOrByte` concept is defined in `mathematical_result.h`

## Sub With Borrow (`sub_with_borrow.h`)
```cpp
namespace UtilityLib {

constexpr auto SubWithBorrow = [](IntegralOrByte auto left, IntegralOrByte auto right) constexpr noexcept -> MathematicalResult<decltype(left)>;
}
```
- Implemented using Borrow-Lookahead subtractor algorithm
- Returns a `MathematicalResult<T>` (same struct as `AddWithCarry`) with `result` and `carryBits`
- Uses the `IntegralOrByte` concept from `mathematical_result.h`
- Used for `SUB`/`SBC` flag computation

## Bit Mask (`bit_mask.h`)
Compile-time bit mask constants:
```cpp
namespace UtilityLib {
template <uint8_t mask>    // mask must be 0-7
constexpr auto BitMask = static_cast<std::byte>(1 << mask);
}
```
Used to extract specific carry/borrow bit positions:
- `BitMask<3>` = `0x08_b` (half-carry position)
- `BitMask<7>` = `0x80_b` (carry position)
