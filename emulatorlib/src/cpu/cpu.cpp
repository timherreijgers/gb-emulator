/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

module;

#include <cstddef>
#include <cstdint>
#include <exception>

module emulatorlib.cpu;

import :opcode_handler_map;
import utilitylib;

namespace EmulatorLib
{

Cpu::Cpu(AddressBus& bus)
    : m_bus(bus)
    , m_opcodeHandlers(CreateOpcodeHandlers())
    , m_handler(m_opcodeHandlers[0x00].handler(bus, m_registers))
{
    m_registers.instructionRegister = 0x00_b;
    m_registers.accumulator = 0x01_b;
    // TODO: Ensure that the flags are set correctly:
    // https://github.com/gbdev/pandocs/blob/master/src/Power_Up_Sequence.md#cpu-registers
    m_registers.flags = CpuFlags::Zero.AsByte();

    m_registers.programCounter = 0x0100;
    m_registers.stackPointer = 0xFFFE;

    m_registers.bcRegister = 0x0013;
    m_registers.deRegister = 0x00D8;
    m_registers.hlRegister = 0x014D;
    m_registers.wzRegister = 0x0000;
}

void Cpu::Step()
{
    if (m_handler.HasException())
    {
        std::rethrow_exception(m_handler.Exception());
    }

    const auto interruptEnableWasPending = m_registers.masterInterruptState == MasterInterruptState::PENDING;
    m_handler.m_handle.resume();

    if (!m_handler.m_handle.done())
    {
        return;
    }

    if (interruptEnableWasPending && m_registers.masterInterruptState == MasterInterruptState::PENDING)
    {
        m_registers.masterInterruptState = MasterInterruptState::ENABLED;
    }

    const auto instruction = m_bus.ReadFromAddress(m_registers.programCounter);
    m_registers.instructionRegister = instruction;
    m_registers.programCounter++;

    m_handler = m_opcodeHandlers[static_cast<uint8_t>(m_registers.instructionRegister.value)].handler(m_bus, m_registers);
}

auto Cpu::Registers() noexcept -> CpuRegisters&
{
    return m_registers;
}

auto Cpu::Registers() const noexcept -> const CpuRegisters&
{
    return m_registers;
}

} // namespace EmulatorLib
