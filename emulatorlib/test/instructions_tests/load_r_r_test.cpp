/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/address_bus.h"
#include "emulatorlib/byte_utils.h"
#include "emulatorlib/cpu.h"
#include "emulatorlib/instruction_translation.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"

#include "instruction_handlers/register_io_helpers.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

namespace
{

template <typename T>
[[nodiscard]] auto FunctionWrapper8Bit() -> std::function<Register8Bit&(CpuRegisters&)>
{
    return T{};
}

struct InstructionPair
{
    std::byte instruction;
    std::function<Register8Bit&(CpuRegisters&)> destinationRegister;
    std::function<Register8Bit&(CpuRegisters&)> sourceRegister;
};

} // namespace

class LoadRFromRTest : public ::testing::TestWithParam<InstructionPair>
{
protected:
    void SetUpMocks(std::byte instruction)
    {
        ON_CALL(m_addressableMock, ReadFromAddress(0x0100)).WillByDefault(::testing::Return(instruction));
        ON_CALL(m_addressableMock, ReadFromAddress(0x0101)).WillByDefault(::testing::Return(0x00_b));
    }

    ::testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    AddressBus m_bus{{m_addressableMock}};
    Cpu m_cpu{m_bus};
};

TEST_P(LoadRFromRTest, ExecutingOpCode)
{
    const auto& [instruction, destinationRegister, sourceRegister] = GetParam();
    SetUpMocks(instruction);

    // TODO: Registers getter should be const. If we validate that LoadN8A and LoadN16BC works, we can use that to load data into the stack pointer.
    // For this we first need to make sure that we have a better way of creating "programs" than setting up mocks
    destinationRegister(m_cpu.Registers()) = 0x11_b;
    sourceRegister(m_cpu.Registers()) = 0x22_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    m_cpu.Step();
    ASSERT_THAT(destinationRegister(m_cpu.Registers()), ::testing::Eq(0x22_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(LoadRFromRTest, LoadRFromRTest,
                         ::testing::Values(
                             InstructionPair{0x40_b, FunctionWrapper8Bit<RegisterB>(), FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x41_b, FunctionWrapper8Bit<RegisterB>(), FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x42_b, FunctionWrapper8Bit<RegisterB>(), FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x43_b, FunctionWrapper8Bit<RegisterB>(), FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x44_b, FunctionWrapper8Bit<RegisterB>(), FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x45_b, FunctionWrapper8Bit<RegisterB>(), FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x47_b, FunctionWrapper8Bit<RegisterB>(), FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x48_b, FunctionWrapper8Bit<RegisterC>(), FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x49_b, FunctionWrapper8Bit<RegisterC>(), FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x4A_b, FunctionWrapper8Bit<RegisterC>(), FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x4B_b, FunctionWrapper8Bit<RegisterC>(), FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x4C_b, FunctionWrapper8Bit<RegisterC>(), FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x4D_b, FunctionWrapper8Bit<RegisterC>(), FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x4F_b, FunctionWrapper8Bit<RegisterC>(), FunctionWrapper8Bit<RegisterA>()},

                             InstructionPair{0x50_b, FunctionWrapper8Bit<RegisterD>(), FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x51_b, FunctionWrapper8Bit<RegisterD>(), FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x52_b, FunctionWrapper8Bit<RegisterD>(), FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x53_b, FunctionWrapper8Bit<RegisterD>(), FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x54_b, FunctionWrapper8Bit<RegisterD>(), FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x55_b, FunctionWrapper8Bit<RegisterD>(), FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x57_b, FunctionWrapper8Bit<RegisterD>(), FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x58_b, FunctionWrapper8Bit<RegisterE>(), FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x59_b, FunctionWrapper8Bit<RegisterE>(), FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x5A_b, FunctionWrapper8Bit<RegisterE>(), FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x5B_b, FunctionWrapper8Bit<RegisterE>(), FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x5C_b, FunctionWrapper8Bit<RegisterE>(), FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x5D_b, FunctionWrapper8Bit<RegisterE>(), FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x5F_b, FunctionWrapper8Bit<RegisterE>(), FunctionWrapper8Bit<RegisterA>()},

                             InstructionPair{0x60_b, FunctionWrapper8Bit<RegisterH>(), FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x61_b, FunctionWrapper8Bit<RegisterH>(), FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x62_b, FunctionWrapper8Bit<RegisterH>(), FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x63_b, FunctionWrapper8Bit<RegisterH>(), FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x64_b, FunctionWrapper8Bit<RegisterH>(), FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x65_b, FunctionWrapper8Bit<RegisterH>(), FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x67_b, FunctionWrapper8Bit<RegisterH>(), FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x68_b, FunctionWrapper8Bit<RegisterL>(), FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x69_b, FunctionWrapper8Bit<RegisterL>(), FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x6A_b, FunctionWrapper8Bit<RegisterL>(), FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x6B_b, FunctionWrapper8Bit<RegisterL>(), FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x6C_b, FunctionWrapper8Bit<RegisterL>(), FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x6D_b, FunctionWrapper8Bit<RegisterL>(), FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x6F_b, FunctionWrapper8Bit<RegisterL>(), FunctionWrapper8Bit<RegisterA>()},

                             InstructionPair{0x78_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper8Bit<RegisterB>()},
                             InstructionPair{0x79_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper8Bit<RegisterC>()},
                             InstructionPair{0x7A_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper8Bit<RegisterD>()},
                             InstructionPair{0x7B_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper8Bit<RegisterE>()},
                             InstructionPair{0x7C_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper8Bit<RegisterH>()},
                             InstructionPair{0x7D_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper8Bit<RegisterL>()},
                             InstructionPair{0x7F_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper8Bit<RegisterA>()}),
                         [](const testing::TestParamInfo<LoadRFromRTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test