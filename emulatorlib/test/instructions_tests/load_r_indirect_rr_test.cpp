/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "instruction_test_base.h"
#include "emulatorlib/instruction_translation.h"
#include "instruction_handlers/register_io_helpers.h"

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

class LoadRegisterIndirectRegister16BitTest : public InstructionTestBase, public ::testing::WithParamInterface<InstructionPair>
{
protected:
    void SetUp() override
    {
        EXPECT_CALL(m_addressableMock, ReadFromAddress(::testing::Le(0x7FFF)))
            .WillRepeatedly(::testing::Return(0x00_b));
    }
};

TEST_P(LoadRegisterIndirectRegister16BitTest, ExecutingOpCode)
{
    const auto [instruction, registerToLoadTo, registerToLoadFrom, operand] = GetParam();
    m_program.WriteProgram({instruction, 0x00_b});
    registerToLoadTo(m_cpu.Registers()) = 0x00_b;
    registerToLoadFrom(m_cpu.Registers()) = 0x1234;
    const auto originalDestinationValue = registerToLoadTo(m_cpu.Registers()).value;

    m_cpu.Step();
    ASSERT_THAT(m_cpu.Registers().instructionRegister, ::testing::Eq(instruction));

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