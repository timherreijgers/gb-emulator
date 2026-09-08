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

## Add With Carry (`add_with_carry.h`)
```cpp
namespace UtilityLib {

template <UnsignedIntegralOrByte T>
struct AddWithCarryResult<T> {
    T result;
    T carryBits;  // bits that represent carries (position 3 and 7 for half/carry flags)
};

template <UnsignedIntegralOrByte T>
[[nodiscard]] auto AddWithCarry(T left, T right) noexcept -> AddWithCarryResult<T>;

template <UnsignedIntegralOrByte T>
[[nodiscard]] auto AddWithCarryIn(T left, T right, bool carryIn) noexcept -> AddWithCarryResult<T>;
}
```
- Computes `a + b` and returns both result and carry bits
- `AddWithCarryIn` adds an explicit one-bit carry-in to two operands
- Accepts only unsigned integral types or `std::byte`
- Carry bits use bitmask positions (bit 3 for half-carry, bit 7 for carry)

## Sub With Borrow (`sub_with_borrow.h`)
```cpp
namespace UtilityLib {

template <SubWithBorrowOperand T>
struct SubWithBorrowResult<T> {
    T result;
    T borrowBits;  // same bitmask convention as AddWithCarryResult
};

template <SubWithBorrowOperand T>
[[nodiscard]] auto SubWithBorrow(T left, T right) noexcept -> SubWithBorrowResult<T>;
}
```
- Borrow-Lookahead subtractor algorithm
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
