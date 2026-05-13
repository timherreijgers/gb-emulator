/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/address_bus.h"
#include "emulatorlib/byte_utils.h"
#include "emulatorlib/cpu.h"
#include "emulatorlib/instruction_translation.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

namespace
{

struct InstructionPair
{
    std::byte instruction;
    std::function<uint16_t(const CpuRegisters&)> registerGetter;
};

} // namespace

class LoadRegN16Test : public ::testing::TestWithParam<InstructionPair>
{
protected:
    void SetUpMocks(std::byte instruction, uint16_t valueToSet)
    {
        ON_CALL(m_addressableMock, ReadFromAddress(0x0100)).WillByDefault(::testing::Return(instruction));
        ON_CALL(m_addressableMock, ReadFromAddress(0x0101)).WillByDefault(::testing::Return(static_cast<std::byte>(valueToSet & 0xFF)));
        ON_CALL(m_addressableMock, ReadFromAddress(0x0102)).WillByDefault(::testing::Return(static_cast<std::byte>(valueToSet >> 8)));
        ON_CALL(m_addressableMock, ReadFromAddress(0x0103)).WillByDefault(::testing::Return(0x00_b));
    }

    ::testing::NaggyMock<AddressBusAddressableMock> m_addressableMock;
    AddressBus m_bus{{m_addressableMock}};
    Cpu m_cpu{m_bus};
};

TEST_P(LoadRegN16Test, ExecutingCommand_WithValueToLoadAs0x1234_BehavesCorrectly)
{
    const auto& [instructionByte, registerValue] = GetParam();
    const auto defaultValue = registerValue(m_cpu.Registers());

    SetUpMocks(instructionByte, 0x1234);

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instructionByte));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x34_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x34_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x12_b));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(0x1234));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(LoadRegN16Test, ExecutingCommand_WithValueToLoadAs0x5678_BehavesCorrectly)
{
    const auto& [instructionByte, registerValue] = GetParam();
    const auto defaultValue = registerValue(m_cpu.Registers());

    SetUpMocks(instructionByte, 0x5678);

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instructionByte));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x78_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x00_b));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0x78_b));
    ASSERT_THAT(m_cpu.Registers().wRegister, ::testing::Eq(0x56_b));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(0x5678));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(LoadRegN16Test, LoadRegN16Test,
                         ::testing::Values(
                             InstructionPair{0x01_b, [](const CpuRegisters& registers) { return registers.bcRegister.value; }},
                             InstructionPair{0x11_b, [](const CpuRegisters& registers) { return registers.deRegister.value; }},
                             InstructionPair{0x21_b, [](const CpuRegisters& registers) { return registers.hlRegister.value; }},
                             InstructionPair{0x31_b, [](const CpuRegisters& registers) { return registers.stackPointer.value; }}),
                         [](const testing::TestParamInfo<LoadRegN16Test::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test