/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/address_bus.h"
#include "emulatorlib/test/address_bus_addressable_mock.h"
#include "utilitylib/byte_utils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

TEST(AddressBusNoDeviceTest, ReadFromAddress_ReturnsZero)
{
    const AddressBus bus{{}};

    ASSERT_THAT(bus.ReadFromAddress(0x0000), ::testing::Eq(0x00_b));
}

class AddressBusTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ON_CALL(m_addressableMock1, ReadFromAddress(0x0000)).WillByDefault(::testing::Return(0x01_b));
        ON_CALL(m_addressableMock2, ReadFromAddress(0x0001)).WillByDefault(::testing::Return(0x02_b));
    }

    AddressBusAddressableMock m_addressableMock1;
    AddressBusAddressableMock m_addressableMock2;
    AddressBus m_bus{{m_addressableMock1, m_addressableMock2}};
};

TEST_F(AddressBusTest, WriteToAddress_CallsWriteToAddress_OnAllDevices)
{
    EXPECT_CALL(m_addressableMock1, WriteToAddress(0x1000, 0xFF_b)).Times(1);
    EXPECT_CALL(m_addressableMock2, WriteToAddress(0x1000, 0xFF_b)).Times(1);

    m_bus.WriteToAddress(0x1000, 0xFF_b);
}

TEST_F(AddressBusTest, ReadFromAddress_CallsReadFromAddress_OnAllDevices)
{
    EXPECT_CALL(m_addressableMock1, ReadFromAddress(0x1000)).Times(1);
    EXPECT_CALL(m_addressableMock2, ReadFromAddress(0x1000)).Times(1);

    std::ignore = m_bus.ReadFromAddress(0x1000);
}

TEST_F(AddressBusTest, ReadFromAddress_AddressOfAddressable1_ReturnsCorrectValue)
{
    ASSERT_THAT(m_bus.ReadFromAddress(0x0000), ::testing::Eq(0x01_b));
}

TEST_F(AddressBusTest, ReadFromAddress_AddressOfAddressable2_ReturnsCorrectValue)
{
    ASSERT_THAT(m_bus.ReadFromAddress(0x0001), ::testing::Eq(0x02_b));
}

TEST_F(AddressBusTest, ReadFromAddress_BothDevicesRespond_ReturnsCombinedValue)
{
    ON_CALL(m_addressableMock1, ReadFromAddress(0x5000)).WillByDefault(::testing::Return(0x0F_b));
    ON_CALL(m_addressableMock2, ReadFromAddress(0x5000)).WillByDefault(::testing::Return(0xF0_b));

    ASSERT_THAT(m_bus.ReadFromAddress(0x5000), ::testing::Eq(0xFF_b));
}

} // namespace EmulatorLib::Test