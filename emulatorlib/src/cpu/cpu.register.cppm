/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

export module emulatorlib.cpu:registers;

import utilitylib;

export namespace EmulatorLib
{

template <typename T>
struct Register
{
    T& value;
    T mask = static_cast<T>(UtilityLib::numeric_limits<T>::max());

    constexpr auto operator=(std::remove_cvref_t<T> t) -> Register&
    {
        value = (t & mask);
        return *this;
    }

    constexpr operator T() const noexcept
    {
        return value & mask;
    }

    [[nodiscard]] constexpr auto operator+(const T& other) const noexcept -> T
    {
        if constexpr (std::same_as<T, std::byte>)
        {
            const auto lhs = std::to_integer<unsigned int>(value & mask);
            const auto rhs = std::to_integer<unsigned int>(other);
            return static_cast<std::byte>(lhs + rhs) & mask;
        }
        else
        {
            return static_cast<T>((value & mask) + other) & mask;
        }
    }

    constexpr void operator+=(const T& other) noexcept
    {
        if constexpr (std::same_as<T, std::byte>)
        {
            const auto lhs = std::to_integer<unsigned int>(value & mask);
            const auto rhs = std::to_integer<unsigned int>(other);
            value = static_cast<std::byte>(lhs + rhs) & mask;
        }
        else
        {
            value = static_cast<T>((value & mask) + other) & mask;
        }
    }

    [[nodiscard]] constexpr auto operator-(const T& other) const noexcept -> T
    {
        if constexpr (std::same_as<T, std::byte>)
        {
            const auto lhs = std::to_integer<unsigned int>(value & mask);
            const auto rhs = std::to_integer<unsigned int>(other);
            return static_cast<std::byte>(lhs - rhs) & mask;
        }
        else
        {
            return static_cast<T>((value & mask) - other) & mask;
        }
    }

    constexpr void operator-=(const T& other) noexcept
    {
        if constexpr (std::same_as<T, std::byte>)
        {
            const auto lhs = std::to_integer<unsigned int>(value & mask);
            const auto rhs = std::to_integer<unsigned int>(other);
            value = static_cast<std::byte>(lhs - rhs) & mask;
        }
        else
        {
            value = static_cast<T>((value & mask) - other) & mask;
        }
    }

    constexpr auto operator++(int) noexcept -> T
    {
        return value++;
    }

    constexpr auto operator--(int) noexcept -> T
    {
        return value--;
    }

    constexpr auto operator&=(T other) noexcept -> Register<T>&
    {
        value &= other;
        value &= mask;
        return *this;
    }

    constexpr auto operator|=(T other) noexcept -> Register<T>&
    {
        value |= other;
        value &= mask;
        return *this;
    }

    constexpr auto operator^=(T other) noexcept -> Register<T>&
    {
        value ^= other;
        value &= mask;
        return *this;
    }

    [[nodiscard]] constexpr auto operator<=>(const Register<T>& other) const noexcept
    {
        return value <=> other.value;
    }

    [[nodiscard]] constexpr auto operator<=>(const T& other) const noexcept
    {
        return value <=> other;
    }

    [[nodiscard]]
    constexpr auto LowerByteAsRegister() const noexcept
        -> Register<std::byte>
        requires std::same_as<T, std::uint16_t>
    {
        return Register<std::byte>{*reinterpret_cast<std::byte *>(&value), static_cast<std::byte>(mask)};
    }

    [[nodiscard]]
    constexpr auto UpperByteAsRegister() const noexcept
        -> Register<std::byte>
        requires std::same_as<T, std::uint16_t>
    {
        return Register<std::byte>{*(reinterpret_cast<std::byte *>(&value) + 1), static_cast<std::byte>(mask >> 8)};
    }
};

using Register16Bit = Register<uint16_t>;
using Register8Bit = Register<std::byte>;

} // namespace EmulatorLib