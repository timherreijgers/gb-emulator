/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"
#include "emulatorlib/interrupt/interrupt_acknowedgable.h"

#include <array>
#include <functional>
#include <string_view>

namespace EmulatorLib
{

struct OpcodeHandler
{
    std::string_view name;
    std::function<InstructionHandler(AddressBus&, CpuRegisters&)> handler;
};

class Cpu
{
public:
    Cpu(AddressBus& bus, InterruptAcknowledgeable& interruptAcknowledgeable);
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
    InterruptAcknowledgeable& m_interruptAcknowledgeable;
    CpuRegisters m_registers;

    std::array<OpcodeHandler, 256> m_opcodeHandlers;
    std::array<OpcodeHandler, 256> m_extendedOpcodeHandlers;
    InstructionHandler m_handler;
};

} // namespace EmulatorLib