/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cpu.h"

#include "emulatorlib/byte_utils.h"

namespace EmulatorLib
{

namespace
{

InstructionHandler UnimplementedOpcode(AddressBus& /*addressBus*/, CpuRegisters& /*cpuRegisters*/)
{
    throw std::runtime_error("Unimplemented opcode");
}

InstructionHandler ExecuteNoop(AddressBus& /*addressBus*/, CpuRegisters& /*cpuRegisters*/)
{
    co_return;
}

template <typename Register>
[[nodiscard]] auto ExecuteLoadRegN8(AddressBus& addressBus, CpuRegisters& cpuRegisters, Register& reg) -> InstructionHandler
{
    // Load byte from memory into register Z
    const auto val = addressBus.ReadFromAddress(cpuRegisters.programCounter++);
    cpuRegisters.zRegister = val;
    co_yield std::monostate{};

    // Load byte from memory into register Z into register B
    reg = cpuRegisters.zRegister.value;
    co_return;
}

InstructionHandler ExecuteLoadBn8(AddressBus& addressBus, CpuRegisters& cpuRegisters)
{
    return ExecuteLoadRegN8(addressBus, cpuRegisters, cpuRegisters.bRegister);
}

InstructionHandler ExecuteLoadCn8(AddressBus& addressBus, CpuRegisters& cpuRegisters)
{
    return ExecuteLoadRegN8(addressBus, cpuRegisters, cpuRegisters.cRegister);
}

InstructionHandler ExecuteLoadDn8(AddressBus& addressBus, CpuRegisters& cpuRegisters)
{
    return ExecuteLoadRegN8(addressBus, cpuRegisters, cpuRegisters.dRegister);
}

InstructionHandler ExecuteLoadEn8(AddressBus& addressBus, CpuRegisters& cpuRegisters)
{
    return ExecuteLoadRegN8(addressBus, cpuRegisters, cpuRegisters.eRegister);
}

InstructionHandler ExecuteLoadHn8(AddressBus& addressBus, CpuRegisters& cpuRegisters)
{
    return ExecuteLoadRegN8(addressBus, cpuRegisters, cpuRegisters.hRegister);
}

InstructionHandler ExecuteLoadLn8(AddressBus& addressBus, CpuRegisters& cpuRegisters)
{
    return ExecuteLoadRegN8(addressBus, cpuRegisters, cpuRegisters.lRegister);
}

InstructionHandler ExecuteLoadAn8(AddressBus& addressBus, CpuRegisters& cpuRegisters)
{
    return ExecuteLoadRegN8(addressBus, cpuRegisters, cpuRegisters.accumulator);
}

[[nodiscard]] constexpr auto CreateOpcodeHandlers() noexcept -> std::array<std::function<InstructionHandler(AddressBus&, CpuRegisters&)>, 256>
{
    std::array<std::function<InstructionHandler(AddressBus&, CpuRegisters&)>, 256> handlers{};
    handlers.fill(UnimplementedOpcode);

    handlers[0x00] = ExecuteNoop;
    handlers[0x06] = ExecuteLoadBn8;
    handlers[0x0E] = ExecuteLoadCn8;
    handlers[0x16] = ExecuteLoadDn8;
    handlers[0x1E] = ExecuteLoadEn8;
    handlers[0x26] = ExecuteLoadHn8;
    handlers[0x2E] = ExecuteLoadLn8;
    handlers[0x3E] = ExecuteLoadAn8;

    return handlers;
}

std::array<std::function<InstructionHandler(AddressBus&, CpuRegisters&)>, 256> g_opcodeHandlers{};


} // namespace

CpuFlags CpuFlags::Zero(1 << 7);
CpuFlags CpuFlags::Subtract(1 << 6);
CpuFlags CpuFlags::HalfCarry(1 << 5);
CpuFlags CpuFlags::Carry(1 << 4);

Cpu::Cpu(AddressBus& bus)
    : m_bus(bus)
{
    m_registers.instructionRegister = 0x00_b;
    m_registers.accumulator = 0x01_b;
    m_registers.flags = CpuFlags::Zero.AsByte();

    m_registers.programCounter = 0x0100;
    m_registers.stackPointer = 0xFFFE;

    m_registers.bcRegister = 0x0013;
    m_registers.deRegister = 0x00D8;
    m_registers.hlRegister = 0x014D;
    m_registers.wzRegister = 0x0000;

    g_opcodeHandlers = CreateOpcodeHandlers();

    m_handler = g_opcodeHandlers[0x00](bus, m_registers);
}

void Cpu::Step()
{
    if (m_handler->m_handle.done())
    {
        m_handler = g_opcodeHandlers[static_cast<uint8_t>(m_registers.instructionRegister.value)](m_bus, m_registers);
    }

    m_handler->m_handle.resume();

    if (!m_handler->m_handle.done())
    {
        return;
    }

    const auto instruction = m_bus.ReadFromAddress(m_registers.programCounter);
    m_registers.instructionRegister = instruction;
    m_registers.programCounter += 1;
}

auto Cpu::Registers() const noexcept -> const CpuRegisters&
{
    return m_registers;
}

} // namespace EmulatorLib