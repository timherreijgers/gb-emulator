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
[[nodiscard]] auto FunctionWrapper16Bit() -> std::function<Register16Bit&(CpuRegisters&)>
{
    return T{};
}

template <typename T>
[[nodiscard]] auto FunctionWrapper8Bit() -> std::function<Register8Bit&(CpuRegisters&)>
{
    return T{};
}

struct InstructionPair
{
    std::byte instruction;
    std::function<Register16Bit&(CpuRegisters&)> destinationRegister;
    std::function<Register8Bit&(CpuRegisters&)> sourceRegister;
};

} // namespace

class LoadIndirectATest : public ::testing::TestWithParam<InstructionPair>
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

TEST_P(LoadIndirectATest, ExecutingCommand_WithValueToLoadAs0x1234_BehavesCorrectly)
{
    const auto& [instruction, destinationRegister, sourceRegister] = GetParam();
    SetUpMocks(instruction);

    // TODO: Registers getter should be const. If we validate that LoadN8A and LoadN16BC works, we can use that to load data into the stack pointer.
    // For this we first need to make sure that we have a better way of creating "programs" than setting up mocks
    destinationRegister(m_cpu.Registers()) = 0x1234;
    sourceRegister(m_cpu.Registers()) = 0xFF_b;
    // m_cpu.Registers().accumulator = 0xFF_b;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    EXPECT_CALL(m_addressableMock, WriteToAddress(0x1234, 0xFF_b)).Times(1);
    m_cpu.Step();

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(LoadIndirectATest, LoadIndirectATest,
                         ::testing::Values(
                             InstructionPair{0x02_b, FunctionWrapper16Bit<RegisterBC>(), FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x12_b, FunctionWrapper16Bit<RegisterDE>(), FunctionWrapper8Bit<RegisterA>()},
                             InstructionPair{0x77_b, FunctionWrapper16Bit<RegisterHL>(), FunctionWrapper8Bit<RegisterA>()}),
                         [](const testing::TestParamInfo<LoadIndirectATest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test