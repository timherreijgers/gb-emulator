/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu.h"

#include "emulatorlib/byte_utils.h"
#include "emulatorlib/cpu_flags.h"

#include "instruction_handlers/opcode_handler_map.h"

namespace EmulatorLib
{

namespace
{

[[nodiscard]] auto DefaultInitializeRegisters() noexcept -> CpuRegisters
{
    CpuRegisters registers{};

    registers.instructionRegister = 0x00_b;
    registers.accumulator = 0x01_b;
    registers.flags = CpuFlags::Zero.AsByte();

    registers.programCounter = 0x0100;
    registers.stackPointer = 0xFFFE;

    registers.bcRegister = 0x0013;
    registers.deRegister = 0x00D8;
    registers.hlRegister = 0x014D;
    registers.wzRegister = 0x0000;

    return registers;
}

} // namespace

Cpu::Cpu(AddressBus& bus)
    : m_bus(bus)
    , m_registers(DefaultInitializeRegisters())
    , m_opcodeHandlers(CreateOpcodeHandlers())
    , m_handler(m_opcodeHandlers[0x00](bus, m_registers))
{
}

void Cpu::Step()
{
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

auto Cpu::Registers() const noexcept -> const CpuRegisters&
{
    return m_registers;
}

} // namespace EmulatorLib