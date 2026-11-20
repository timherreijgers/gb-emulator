/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu_registers.h"
#include "emulatorlib/instruction_handler.h"

#include "instruction_handlers/register_concepts.h"
#include "instruction_handlers/register_io_helpers.h"
#include "utilitylib/bit_mask.h"

namespace EmulatorLib
{

template <ReturnsRegister8Bit TargetRegister, uint8_t Bit>
    requires UtilityLib::ValidBitMask<Bit>
constexpr auto ExecuteRes = [](const AddressBus& /*addressBus*/, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    TargetRegister{}(cpuRegisters) &= ~UtilityLib::BitMask<Bit>;

    co_return;
};

constexpr auto ExecuteRes0B = ExecuteRes<RegisterB, 0>;
constexpr auto ExecuteRes0C = ExecuteRes<RegisterC, 0>;
constexpr auto ExecuteRes0D = ExecuteRes<RegisterD, 0>;
constexpr auto ExecuteRes0E = ExecuteRes<RegisterE, 0>;
constexpr auto ExecuteRes0H = ExecuteRes<RegisterH, 0>;
constexpr auto ExecuteRes0L = ExecuteRes<RegisterL, 0>;
constexpr auto ExecuteRes0A = ExecuteRes<RegisterA, 0>;
constexpr auto ExecuteRes1B = ExecuteRes<RegisterB, 1>;
constexpr auto ExecuteRes1C = ExecuteRes<RegisterC, 1>;
constexpr auto ExecuteRes1D = ExecuteRes<RegisterD, 1>;
constexpr auto ExecuteRes1E = ExecuteRes<RegisterE, 1>;
constexpr auto ExecuteRes1H = ExecuteRes<RegisterH, 1>;
constexpr auto ExecuteRes1L = ExecuteRes<RegisterL, 1>;
constexpr auto ExecuteRes1A = ExecuteRes<RegisterA, 1>;
constexpr auto ExecuteRes2B = ExecuteRes<RegisterB, 2>;
constexpr auto ExecuteRes2C = ExecuteRes<RegisterC, 2>;
constexpr auto ExecuteRes2D = ExecuteRes<RegisterD, 2>;
constexpr auto ExecuteRes2E = ExecuteRes<RegisterE, 2>;
constexpr auto ExecuteRes2H = ExecuteRes<RegisterH, 2>;
constexpr auto ExecuteRes2L = ExecuteRes<RegisterL, 2>;
constexpr auto ExecuteRes2A = ExecuteRes<RegisterA, 2>;
constexpr auto ExecuteRes3B = ExecuteRes<RegisterB, 3>;
constexpr auto ExecuteRes3C = ExecuteRes<RegisterC, 3>;
constexpr auto ExecuteRes3D = ExecuteRes<RegisterD, 3>;
constexpr auto ExecuteRes3E = ExecuteRes<RegisterE, 3>;
constexpr auto ExecuteRes3H = ExecuteRes<RegisterH, 3>;
constexpr auto ExecuteRes3L = ExecuteRes<RegisterL, 3>;
constexpr auto ExecuteRes3A = ExecuteRes<RegisterA, 3>;
constexpr auto ExecuteRes4B = ExecuteRes<RegisterB, 4>;
constexpr auto ExecuteRes4C = ExecuteRes<RegisterC, 4>;
constexpr auto ExecuteRes4D = ExecuteRes<RegisterD, 4>;
constexpr auto ExecuteRes4E = ExecuteRes<RegisterE, 4>;
constexpr auto ExecuteRes4H = ExecuteRes<RegisterH, 4>;
constexpr auto ExecuteRes4L = ExecuteRes<RegisterL, 4>;
constexpr auto ExecuteRes4A = ExecuteRes<RegisterA, 4>;
constexpr auto ExecuteRes5B = ExecuteRes<RegisterB, 5>;
constexpr auto ExecuteRes5C = ExecuteRes<RegisterC, 5>;
constexpr auto ExecuteRes5D = ExecuteRes<RegisterD, 5>;
constexpr auto ExecuteRes5E = ExecuteRes<RegisterE, 5>;
constexpr auto ExecuteRes5H = ExecuteRes<RegisterH, 5>;
constexpr auto ExecuteRes5L = ExecuteRes<RegisterL, 5>;
constexpr auto ExecuteRes5A = ExecuteRes<RegisterA, 5>;
constexpr auto ExecuteRes6B = ExecuteRes<RegisterB, 6>;
constexpr auto ExecuteRes6C = ExecuteRes<RegisterC, 6>;
constexpr auto ExecuteRes6D = ExecuteRes<RegisterD, 6>;
constexpr auto ExecuteRes6E = ExecuteRes<RegisterE, 6>;
constexpr auto ExecuteRes6H = ExecuteRes<RegisterH, 6>;
constexpr auto ExecuteRes6L = ExecuteRes<RegisterL, 6>;
constexpr auto ExecuteRes6A = ExecuteRes<RegisterA, 6>;
constexpr auto ExecuteRes7B = ExecuteRes<RegisterB, 7>;
constexpr auto ExecuteRes7C = ExecuteRes<RegisterC, 7>;
constexpr auto ExecuteRes7D = ExecuteRes<RegisterD, 7>;
constexpr auto ExecuteRes7E = ExecuteRes<RegisterE, 7>;
constexpr auto ExecuteRes7H = ExecuteRes<RegisterH, 7>;
constexpr auto ExecuteRes7L = ExecuteRes<RegisterL, 7>;
constexpr auto ExecuteRes7A = ExecuteRes<RegisterA, 7>;

template <uint8_t Bit>
    requires UtilityLib::ValidBitMask<Bit>
constexpr auto ExecuteResIndirectHL = [](AddressBus& addressBus, CpuRegisters& cpuRegisters) noexcept -> InstructionHandler {
    cpuRegisters.zRegister = addressBus.ReadFromAddress(cpuRegisters.hlRegister);
    co_yield std::monostate{};

    cpuRegisters.zRegister &= ~UtilityLib::BitMask<Bit>;
    addressBus.WriteToAddress(cpuRegisters.hlRegister, cpuRegisters.zRegister);

    co_return;
};

constexpr auto ExecuteRes0IndirectHL = ExecuteResIndirectHL<0>;
constexpr auto ExecuteRes1IndirectHL = ExecuteResIndirectHL<1>;
constexpr auto ExecuteRes2IndirectHL = ExecuteResIndirectHL<2>;
constexpr auto ExecuteRes3IndirectHL = ExecuteResIndirectHL<3>;
constexpr auto ExecuteRes4IndirectHL = ExecuteResIndirectHL<4>;
constexpr auto ExecuteRes5IndirectHL = ExecuteResIndirectHL<5>;
constexpr auto ExecuteRes6IndirectHL = ExecuteResIndirectHL<6>;
constexpr auto ExecuteRes7IndirectHL = ExecuteResIndirectHL<7>;

} // namespace EmulatorLib
