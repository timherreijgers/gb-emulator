/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/load_a16_sp.h"
#include "instruction_handlers/load_indirect_a.h"
#include "instruction_handlers/load_r_n8.h"
#include "instruction_handlers/load_rr_n16.h"

#include <stdexcept>

namespace EmulatorLib
{

InstructionHandler UnimplementedOpcode(AddressBus& /*addressBus*/, CpuRegisters& /*cpuRegisters*/)
{
    throw std::runtime_error("Unimplemented opcode");
}

InstructionHandler ExecuteNoop(AddressBus& /*addressBus*/, CpuRegisters& /*cpuRegisters*/)
{
    co_return;
}

[[nodiscard]] constexpr auto CreateOpcodeHandlers() noexcept -> std::array<std::function<InstructionHandler(AddressBus&, CpuRegisters&)>, 256>
{
    std::array<std::function<InstructionHandler(AddressBus&, CpuRegisters&)>, 256> handlers{};
    handlers.fill(UnimplementedOpcode);

    handlers[0x00] = ExecuteNoop;
    handlers[0x01] = ExecuteLoadBCn16;
    handlers[0x02] = ExecuteLoadIndirectBCFromA;
    handlers[0x06] = ExecuteLoadBn8;
    handlers[0x08] = ExecuteLoadA16SP;
    handlers[0x0E] = ExecuteLoadCn8;

    handlers[0x11] = ExecuteLoadDEn16;
    handlers[0x12] = ExecuteLoadIndirectDEFromA;
    handlers[0x16] = ExecuteLoadDn8;
    handlers[0x1E] = ExecuteLoadEn8;

    handlers[0x21] = ExecuteLoadHLn16;
    handlers[0x26] = ExecuteLoadHn8;
    handlers[0x2E] = ExecuteLoadLn8;

    handlers[0x31] = ExecuteLoadSPn16;
    handlers[0x3E] = ExecuteLoadAn8;

    return handlers;
}

} // namespace EmulatorLib