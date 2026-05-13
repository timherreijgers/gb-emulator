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
    std::function<std::byte(const CpuRegisters&)> registerGetter;
};

} // namespace

class LoadRegN8Test : public ::testing::TestWithParam<InstructionPair>
{
protected:
    void SetUpMocks(std::byte instruction, std::byte valueToSet)
    {
        ON_CALL(m_addressableMock, ReadFromAddress(0x0100)).WillByDefault(::testing::Return(instruction));
        ON_CALL(m_addressableMock, ReadFromAddress(0x0101)).WillByDefault(::testing::Return(valueToSet));
        ON_CALL(m_addressableMock, ReadFromAddress(0x0102)).WillByDefault(::testing::Return(0x00_b));
    }

    ::testing::NaggyMock<AddressBusAddressableMock> m_addressableMock;
    AddressBus m_bus{{m_addressableMock}};
    Cpu m_cpu{m_bus};
};

TEST_P(LoadRegN8Test, ExecutingCommand_WithValueToLoadAs0xFF_BehavesCorrectly)
{
    const auto& [instructionByte, registerValue] = GetParam();
    const auto defaultValue = registerValue(m_cpu.Registers());

    SetUpMocks(instructionByte, 0xFF_b);

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instructionByte));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(0xFF_b));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(0xFF_b));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

TEST_P(LoadRegN8Test, ExecutingCommand_WithValueToLoadAs0xFE_BehavesCorrectly)
{
    const auto& [instructionByte, registerValue] = GetParam();
    const auto defaultValue = registerValue(m_cpu.Registers());
    const auto valueToLoad = 0xFE_b;

    SetUpMocks(instructionByte, valueToLoad);

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instructionByte));

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().zRegister, ::testing::Eq(valueToLoad));
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(defaultValue));

    m_cpu.Step();
    ASSERT_THAT(registerValue(m_cpu.Registers()), ::testing::Eq(valueToLoad));
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(LoadRegN8Test, LoadRegN8Test,
                         ::testing::Values(
                             InstructionPair{0x06_b, [](const CpuRegisters& registers) {
                                                 return registers.bRegister.value;
                                             }},
                             InstructionPair{0x0E_b, [](const CpuRegisters& registers) {
                                                 return registers.cRegister.value;
                                             }},
                             InstructionPair{0x16_b, [](const CpuRegisters& registers) {
                                                 return registers.dRegister.value;
                                             }},
                             InstructionPair{0x1E_b, [](const CpuRegisters& registers) {
                                                 return registers.eRegister.value;
                                             }},
                             InstructionPair{0x26_b, [](const CpuRegisters& registers) {
                                                 return registers.hRegister.value;
                                             }},
                             InstructionPair{0x2E_b, [](const CpuRegisters& registers) {
                                                 return registers.lRegister.value;
                                             }},
                             InstructionPair{0x3E_b, [](const CpuRegisters& registers) {
                                                 return registers.accumulator.value;
                                             }}),
                         [](const testing::TestParamInfo<LoadRegN8Test::ParamType>& info) {
                             return std::string{OpCodeToInstructionName(info.param.instruction)};
                         });

} // namespace EmulatorLib::Test