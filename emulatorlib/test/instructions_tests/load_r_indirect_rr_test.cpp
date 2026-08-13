/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/address_bus.h"
#include "emulatorlib/cpu.h"
#include "emulatorlib/instruction_translation.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"
#include "instruction_handlers/register_io_helpers.h"
#include "utilitylib/byte_utils.h"

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

template <typename T>
[[nodiscard]] auto FunctionWrapper16Bit() -> std::function<Register16Bit&(CpuRegisters&)>
{
    return T{};
}

struct InstructionPair
{
    std::byte instruction;
    std::function<Register8Bit&(CpuRegisters&)> destinationRegister;
    std::function<Register16Bit&(CpuRegisters&)> sourceRegister;
    std::function<uint16_t(uint16_t value)> operand = [](auto val) {
        return val;
    };
};

} // namespace

class LoadRegisterIndirectRegister16BitTest : public ::testing::TestWithParam<InstructionPair>
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

TEST_P(LoadRegisterIndirectRegister16BitTest, ExecutingOpCode)
{
    const auto [instruction, registerToLoadTo, registerToLoadFrom, operand] = GetParam();
    SetUpMocks(instruction);
    registerToLoadTo(m_cpu.Registers()) = 0x00_b;
    registerToLoadFrom(m_cpu.Registers()) = 0x1234;
    const auto originalDestinationValue = registerToLoadTo(m_cpu.Registers()).value;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

    // TODO: We have to set a wildcard for EXPECT_CALL to ensure the test doesn't fail. When I have a better way to create
    // programs this should not be an issue anymore.
    EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::_)).Times(::testing::AnyNumber());
    EXPECT_CALL(m_addressableMock, ReadFromAddress(0x1234)).Times(1).WillOnce(::testing::Return(0xAA_b));
    m_cpu.Step();
    ASSERT_THAT(registerToLoadTo(m_cpu.Registers()).value, ::testing::Eq(originalDestinationValue));
    ASSERT_THAT(registerToLoadFrom(m_cpu.Registers()).value, ::testing::Eq(operand(0x1234)));

    m_cpu.Step();
    ASSERT_THAT(registerToLoadTo(m_cpu.Registers()).value, ::testing::Eq(0xAA_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(LoadRegisterHlIndirectTest, LoadRegisterIndirectRegister16BitTest,
                         ::testing::Values(
                             InstructionPair{0x0A_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper16Bit<RegisterBC>()},
                             InstructionPair{0x1A_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper16Bit<RegisterDE>()},
                             InstructionPair{0x2A_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper16Bit<RegisterHL>(), [](auto val) {
                                                 return val + 1;
                                             }},
                             InstructionPair{0x3A_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper16Bit<RegisterHL>(), [](auto val) {
                                                 return val - 1;
                                             }},
                             InstructionPair{0x46_b, FunctionWrapper8Bit<RegisterB>(), FunctionWrapper16Bit<RegisterHL>()}, InstructionPair{0x4E_b, FunctionWrapper8Bit<RegisterC>(), FunctionWrapper16Bit<RegisterHL>()}, InstructionPair{0x56_b, FunctionWrapper8Bit<RegisterD>(), FunctionWrapper16Bit<RegisterHL>()}, InstructionPair{0x5E_b, FunctionWrapper8Bit<RegisterE>(), FunctionWrapper16Bit<RegisterHL>()}, InstructionPair{0x66_b, FunctionWrapper8Bit<RegisterH>(), FunctionWrapper16Bit<RegisterHL>()}, InstructionPair{0x6E_b, FunctionWrapper8Bit<RegisterL>(), FunctionWrapper16Bit<RegisterHL>()}, InstructionPair{0x7E_b, FunctionWrapper8Bit<RegisterA>(), FunctionWrapper16Bit<RegisterHL>()}),
                         [](const testing::TestParamInfo<LoadRegisterIndirectRegister16BitTest::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });


} // namespace EmulatorLib::Test