/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus_addressable.h"
#include "utilitylib/byte_utils.h"

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <vector>

namespace EmulatorLib::Test
{

class ProgramStub : public AddressBusAddressable
{
public:
    explicit constexpr ProgramStub(size_t offset)
        : m_offset(offset)
    {
    }

    constexpr void WriteProgram(std::initializer_list<std::byte> program)
    {
        m_program = std::vector(program);
    }

    constexpr void WriteToAddress(uint16_t /*address*/, std::byte /*data*/) noexcept override
    {
    }

    [[nodiscard]] constexpr auto ReadFromAddress(uint16_t address) const noexcept -> std::byte override
    {
        if (address - m_offset >= m_program.size())
            return 0x00_b;

        return m_program[address - m_offset];
    }

private:
    std::vector<std::byte> m_program;
    size_t m_offset;
};

} // namespace EmulatorLib::Test