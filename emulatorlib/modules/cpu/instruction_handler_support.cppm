/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

export module emulatorlib.cpu:instruction_handler_support;

import std;

import utilitylib;
import :register_;
import :registers;
import :flags;

export namespace EmulatorLib
{

struct RegisterA
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register8Bit&
    {
        return registers.accumulator;
    }
};

struct RegisterB
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register8Bit&
    {
        return registers.bRegister;
    }
};

struct RegisterC
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register8Bit&
    {
        return registers.cRegister;
    }
};

struct RegisterD
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register8Bit&
    {
        return registers.dRegister;
    }
};

struct RegisterE
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register8Bit&
    {
        return registers.eRegister;
    }
};

struct RegisterH
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register8Bit&
    {
        return registers.hRegister;
    }
};

struct RegisterL
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register8Bit&
    {
        return registers.lRegister;
    }
};

struct RegisterZ
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register8Bit&
    {
        return registers.zRegister;
    }
};

struct RegisterAF
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register16Bit&
    {
        return registers.afRegister;
    }
};

struct RegisterBC
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register16Bit&
    {
        return registers.bcRegister;
    }
};

struct RegisterDE
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register16Bit&
    {
        return registers.deRegister;
    }
};

struct RegisterHL
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register16Bit&
    {
        return registers.hlRegister;
    }
};

struct RegisterSP
{
    constexpr auto operator()(CpuRegisters& registers) const noexcept -> Register16Bit&
    {
        return registers.stackPointer;
    }
};

struct Register16BitIncrement
{
    constexpr auto operator()(Register16Bit& reg) const noexcept -> void
    {
        reg.value++;
    }
};

struct Register16BitDecrement
{
    constexpr auto operator()(Register16Bit& reg) const noexcept -> void
    {
        reg.value--;
    }
};

struct RegisterNoModifier
{
    constexpr auto operator()(Register16Bit& /*reg*/) const noexcept -> void
    {
    }
};

} // namespace EmulatorLib

export namespace EmulatorLib
{

template <typename T>
concept ReturnsRegister8Bit = requires(T reg, CpuRegisters& registers) {
    { reg(registers) } -> std::same_as<Register8Bit&>;
};

template <typename T>
concept ReturnsRegister16Bit = requires(T reg, CpuRegisters& registers) {
    { reg(registers) } -> std::same_as<Register16Bit&>;
};

template <typename T>
concept RegisterModifier8Bit = requires(T t, Register8Bit reg) {
    { t(reg) } -> std::same_as<void>;
};

template <typename T>
concept RegisterModifier16Bit = requires(T t, Register16Bit reg) {
    { t(reg) } -> std::same_as<void>;
};

template <typename T>
concept RegisterModifier = RegisterModifier8Bit<T> || RegisterModifier16Bit<T>;

} // namespace EmulatorLib

export namespace EmulatorLib
{

struct AlwaysTrueCallable
{
    constexpr auto operator()(const CpuRegisters&) const noexcept -> bool
    {
        return true;
    }
};

inline constexpr AlwaysTrueCallable AlwaysTrue{};

struct NonZeroCallable
{
    constexpr auto operator()(const CpuRegisters& cpuRegisters) const noexcept -> bool
    {
        return (cpuRegisters.flags.value & CpuFlags::Zero.AsByte()) == 0x00_b;
    }
};

inline constexpr NonZeroCallable NonZero{};

struct ZeroCallable
{
    constexpr auto operator()(const CpuRegisters& cpuRegisters) const noexcept -> bool
    {
        return (cpuRegisters.flags.value & CpuFlags::Zero.AsByte()) > 0x00_b;
    }
};

inline constexpr ZeroCallable Zero{};

struct NonCarryCallable
{
    constexpr auto operator()(const CpuRegisters& cpuRegisters) const noexcept -> bool
    {
        return (cpuRegisters.flags.value & CpuFlags::Carry.AsByte()) == 0x00_b;
    }
};

inline constexpr NonCarryCallable NonCarry{};

struct CarryCallable
{
    constexpr auto operator()(const CpuRegisters& cpuRegisters) const noexcept -> bool
    {
        return (cpuRegisters.flags.value & CpuFlags::Carry.AsByte()) > 0x00_b;
    }
};

inline constexpr CarryCallable Carry{};

template <typename T>
concept CpuRegisterPredicate = std::invocable<T, const CpuRegisters&> &&
                               std::same_as<std::invoke_result_t<T, const CpuRegisters&>, bool>;

} // namespace EmulatorLib

export namespace EmulatorLib
{

[[nodiscard]] constexpr auto CarryIn(const CpuRegisters& registers) noexcept -> bool
{
    return (registers.flags.value & CpuFlags::Carry.AsByte()) > 0x00_b;
}

struct ApplyAdditionFlagsCallable
{
    constexpr auto operator()(CpuRegisters& registers, const std::byte result, const std::byte carryPerBit) const noexcept -> void
    {
        registers.flags.value = (result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b) |
                                ((carryPerBit & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b) |
                                ((carryPerBit & UtilityLib::BitMask<7>) > 0x00_b ? CpuFlags::Carry.AsByte() : 0x00_b);
    }
};

inline constexpr ApplyAdditionFlagsCallable ApplyAdditionFlags{};

struct ApplySubtractionFlagsCallable
{
    constexpr auto operator()(CpuRegisters& registers, const std::byte result, const std::byte carryPerBit) const noexcept -> void
    {
        registers.flags.value = CpuFlags::Subtract.AsByte() |
                                (result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b) |
                                ((carryPerBit & UtilityLib::BitMask<3>) > 0x00_b ? CpuFlags::HalfCarry.AsByte() : 0x00_b) |
                                ((carryPerBit & UtilityLib::BitMask<7>) > 0x00_b ? CpuFlags::Carry.AsByte() : 0x00_b);
    }
};

inline constexpr ApplySubtractionFlagsCallable ApplySubtractionFlags{};

struct ApplyAndFlagsCallable
{
    constexpr auto operator()(CpuRegisters& registers, const std::byte result, const std::byte /*carry*/) const noexcept -> void
    {
        registers.flags.value = (result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b) | CpuFlags::HalfCarry.AsByte();
    }
};

inline constexpr ApplyAndFlagsCallable ApplyAndFlags{};

struct ApplyOrFlagsCallable
{
    constexpr auto operator()(CpuRegisters& registers, const std::byte result, const std::byte /*carry*/) const noexcept -> void
    {
        registers.flags.value = (result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b);
    }
};

inline constexpr ApplyOrFlagsCallable ApplyOrFlags{};

struct ApplyXorFlagsCallable
{
    constexpr auto operator()(CpuRegisters& registers, const std::byte result, const std::byte /*carry*/) const noexcept -> void
    {
        registers.flags.value = (result == 0x00_b ? CpuFlags::Zero.AsByte() : 0x00_b);
    }
};

inline constexpr ApplyXorFlagsCallable ApplyXorFlags{};

} // namespace EmulatorLib
