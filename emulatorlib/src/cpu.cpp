/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu.h"

#include "emulatorlib/cpu_flags.h"
#include "utilitylib/byte_utils.h"

#include "instruction_handlers/opcode_handler_map.h"

namespace EmulatorLib
{

Cpu::Cpu(AddressBus& bus)
    : m_bus(bus)
    , m_opcodeHandlers(CreateOpcodeHandlers())
    , m_handler(m_opcodeHandlers[0x00](bus, m_registers))
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

    m_handler.m_handle.resume();

    if (!m_handler.m_handle.done())
    {
        return;
    }

    const auto instruction = m_bus.ReadFromAddress(m_registers.programCounter);
    m_registers.instructionRegister = instruction;
    m_registers.programCounter++;

    m_handler = m_opcodeHandlers[static_cast<uint8_t>(m_registers.instructionRegister.value)](m_bus, m_registers);
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