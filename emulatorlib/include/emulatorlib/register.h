/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "utilitylib/byte_utils.h"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace EmulatorLib
{

template <typename T>
struct Register
{
    T& value;
    T mask = static_cast<T>(UtilityLib::numeric_limits<T>::max());

    auto operator=(std::remove_cvref_t<T> t) -> Register&
    {
        value = (t & mask);
        return *this;
    }

    operator T() const noexcept
    {
        return value & mask;
    }

    [[nodiscard]] auto operator+(const T& other) const noexcept -> T
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

    void operator+=(const T& other) noexcept
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

    auto operator++(int) noexcept -> T
    {
        return value++;
    }

    auto operator--(int) noexcept -> T
    {
        return value--;
    }

    auto operator&=(T other) noexcept -> Register<T>&
    {
        value &= other;
        value &= mask;
        return *this;
    }

    auto operator|=(T other) noexcept -> Register<T>&
    {
        value |= other;
        value &= mask;
        return *this;
    }

    [[nodiscard]] auto operator<=>(const Register<T>& other) const noexcept
    {
        return value <=> other.value;
    }

    [[nodiscard]] auto operator<=>(const T& other) const noexcept
    {
        return value <=> other;
    }

    [[nodiscard]]
    auto LowerByteAsRegister()
        -> Register<std::byte>
        requires std::same_as<T, std::uint16_t>
    {
        return Register<std::byte>{*reinterpret_cast<std::byte *>(&value), static_cast<std::byte>(mask)};
    }

    [[nodiscard]]
    auto UpperByteAsRegister()
        -> Register<std::byte>
        requires std::same_as<T, std::uint16_t>
    {
        return Register<std::byte>{*(reinterpret_cast<std::byte *>(&value) + 1), static_cast<std::byte>(mask >> 8)};
    }
};

using Register16Bit = Register<uint16_t>;
using Register8Bit = Register<std::byte>;

} // namespace EmulatorLib
