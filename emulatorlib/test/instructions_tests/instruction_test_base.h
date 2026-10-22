#pragma once

import emulatorlib.address_bus;
import emulatorlib.cpu;

#include "mocks/address_bus_addressable_mock.h"
#include "mocks/program_stub.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

class InstructionTestBase : public ::testing::Test
{
protected:
    testing::NiceMock<AddressBusAddressableMock> m_addressableMock;
    ProgramStub m_program{0x100};
    AddressBus m_bus{{m_addressableMock, m_program}};
    Cpu m_cpu{m_bus};
};

} // namespace EmulatorLib::Test