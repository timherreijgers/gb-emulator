/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include <cstdint>
#include <utility>

namespace EmulatorLib
{

template <typename T>
struct Register
{
    T& value;

    auto operator=(std::remove_cvref_t<T> t) -> Register<T>&
    {
        value = t;
        return *this;
    }

    operator T() const noexcept
    {
        return value;
    }

    [[nodiscard]] auto operator+(const T& other) const noexcept -> T
    {
        return value + other;
    }

    void operator+=(const T& other) noexcept
    {
        value += other;
    }

    auto operator++(int) noexcept -> T
    {
        return value++;
    }

    auto operator--(int) noexcept -> T
    {
        return value--;
    }

    [[nodiscard]] auto
        operator<=>(const Register<T>& other) const noexcept
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
        return Register<std::byte>{*reinterpret_cast<std::byte *>(&value)};
    }

    [[nodiscard]]
    auto UpperByteAsRegister()
        -> Register<std::byte>
        requires std::same_as<T, std::uint16_t>
    {
        return Register<std::byte>{*(reinterpret_cast<std::byte *>(&value) + 1)};
    }
};

using Register16Bit = Register<uint16_t>;
using Register8Bit = Register<std::byte>;

} // namespace EmulatorLib