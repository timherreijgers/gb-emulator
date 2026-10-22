/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

import emulatorlib.ram;
import utilitylib;
import std;

using std::uint16_t;


#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

TEST(RamTest, ReadFromAddress_InitialValue_ReturnsZero)
{
    const Ram ram;

    ASSERT_THAT(ram.ReadFromAddress(0xC000), ::testing::Eq(0x00_b));
}

TEST(RamTest, ReadFromAddress_AfterWrite_ReturnsWrittenValue)
{
    Ram ram;

    ram.WriteToAddress(0xC000, 0x42_b);

    ASSERT_THAT(ram.ReadFromAddress(0xC000), ::testing::Eq(0x42_b));
}

class RamParameterizedTest : public ::testing::TestWithParam<uint16_t>
{
};

TEST_P(RamParameterizedTest, AccessOutsideRange_DoesNotAffectRam)
{
    Ram ram;

    ram.WriteToAddress(GetParam(), 0x24_b);

    ASSERT_THAT(ram.ReadFromAddress(GetParam()), ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(RamParameterizedTest, RamParameterizedTest, ::testing::Values(0xBFFF, 0xE000, 0x0000, 0xFFFF));

} // namespace EmulatorLib::Test