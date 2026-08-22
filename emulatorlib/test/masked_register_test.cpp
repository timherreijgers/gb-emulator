/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/register.h"
#include "utilitylib/byte_utils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace EmulatorLib::Test
{

class MaskedRegister8BitTest : public ::testing::TestWithParam<std::byte>
{
};

TEST_P(MaskedRegister8BitTest, Read_AppliesMask)
{
    const auto mask = GetParam();
    auto backing = 0xFF_b;
    Register8Bit reg{backing, mask};

    ASSERT_THAT(static_cast<std::byte>(reg), ::testing::Eq(backing & mask));
}

TEST_P(MaskedRegister8BitTest, Write_OnlyMaskedBitsModified)
{
    const auto mask = GetParam();
    auto backing = 0xAA_b;
    Register8Bit reg{backing, mask};
    const auto expected = std::byte{0xFF} & mask;

    reg = 0xFF_b;

    ASSERT_THAT(backing, ::testing::Eq(expected));
}

TEST_P(MaskedRegister8BitTest, Add_OnlyMaskedBitsAffected)
{
    const auto mask = GetParam();
    auto backing = 0xA0_b;
    Register8Bit reg{backing, mask};
    const auto expected = ((backing & mask) + 0x05_b) & mask;

    reg += 0x05_b;

    ASSERT_THAT(backing, ::testing::Eq(expected));
}

TEST(MaskedRegister8BitTest, Add_DoesNotCarryFromMaskedBits)
{
    auto backing = 0x0F_b;
    Register8Bit reg{backing, 0xF0_b};

    ASSERT_THAT(reg + 0x01_b, ::testing::Eq(0x00_b));
    reg += 0x01_b;
    ASSERT_THAT(backing, ::testing::Eq(0x00_b));
}

INSTANTIATE_TEST_SUITE_P(MaskedRegister8BitTest, MaskedRegister8BitTest, ::testing::Values(0xF0_b, 0x0F_b, 0xFF_b));

class MaskedRegister16BitTest : public ::testing::TestWithParam<uint16_t>
{
};

TEST_P(MaskedRegister16BitTest, Read_AppliesMask)
{
    const auto mask = GetParam();
    auto backing = 0xFFFF_u16;
    Register16Bit reg{backing, mask};

    ASSERT_THAT(static_cast<std::uint16_t>(reg), ::testing::Eq(static_cast<std::uint16_t>(backing & mask)));
}

TEST_P(MaskedRegister16BitTest, Write_OnlyMaskedBitsModified)
{
    const std::uint16_t mask = GetParam();
    auto backing = 0xAA55_u16;
    Register16Bit reg{backing, mask};
    const auto expected = 0xFFFF & mask;

    reg = 0xFFFF_u16;

    ASSERT_THAT(backing, ::testing::Eq(expected));
}

TEST_P(MaskedRegister16BitTest, Add_OnlyMaskedBitsAffected)
{
    const std::uint16_t mask = GetParam();
    auto backing = 0xA000_u16;
    Register16Bit reg{backing, mask};
    const auto expected = static_cast<uint16_t>((backing & mask) + 0x05_u16 & mask);

    reg += 5_u16;

    ASSERT_THAT(backing, ::testing::Eq(expected));
}

TEST(MaskedRegister16BitTest, Add_DoesNotCarryFromMaskedBits)
{
    auto backing = 0x000F_u16;
    Register16Bit reg{backing, 0xFFF0_u16};

    ASSERT_THAT(reg + 1_u16, ::testing::Eq(0_u16));
    reg += 1_u16;
    ASSERT_THAT(backing, ::testing::Eq(0_u16));
}

TEST_P(MaskedRegister16BitTest, BitMask_PropagatesToLowerRegister_ThroughLowerByteAsRegister)
{
    const auto mask = GetParam();
    std::uint16_t backing{};
    Register16Bit reg{backing, GetParam()};

    reg.LowerByteAsRegister() = 0xFF_b;

    ASSERT_THAT(backing, ::testing::Eq(0xFF_u16 & mask));
}

TEST_P(MaskedRegister16BitTest, BitMask_PropagatesToUpperRegister_ThroughUpperByteAsRegister)
{
    const auto mask = GetParam();
    std::uint16_t backing{};
    Register16Bit reg{backing, mask};

    reg.UpperByteAsRegister() = 0xFF_b;

    ASSERT_THAT(backing, ::testing::Eq(0xFF00_u16 & mask));
}

INSTANTIATE_TEST_SUITE_P(MaskedRegister16BitTest, MaskedRegister16BitTest, ::testing::Values(0xFFF0_u16, 0x0FFF_u16, 0xFFFF_u16));

} // namespace EmulatorLib::Test
