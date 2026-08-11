/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include <array>
#include <functional>

namespace EmulatorLib
{

class Cpu
{
public:
    Cpu(AddressBus& bus);
    ~Cpu() = default;
    Cpu(Cpu&) = delete;
    Cpu& operator=(Cpu&) = delete;
    Cpu(Cpu&&) = delete;
    Cpu& operator=(Cpu&&) = delete;

    void Step();

    [[nodiscard]] auto Registers() noexcept -> CpuRegisters&;
    [[nodiscard]] auto Registers() const noexcept -> const CpuRegisters&;

private:
    AddressBus& m_bus;
    CpuRegisters m_registers;

    std::array<std::function<InstructionHandler(AddressBus&, CpuRegisters&)>, 256> m_opcodeHandlers;
    InstructionHandler m_handler;
};

} // namespace EmulatorLib