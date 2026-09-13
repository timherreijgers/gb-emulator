/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "utilitylib/sub_with_borrow.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

namespace UtilityLib::Test
{

namespace
{

struct SubWithBorrowTestParam
{
    uint8_t left;
    uint8_t right;
    uint8_t carryBits;
};

} // namespace

TEST(SubWithBorrowInTest, SubtractingBorrowIn_SetsHalfBorrow)
{
    const auto result = SubWithBorrowIn(uint8_t{0x10}, uint8_t{0x00}, true);

    ASSERT_THAT(result.result, ::testing::Eq(uint8_t{0x0F}));
    ASSERT_THAT(result.carryBits, ::testing::Eq(uint8_t{0x0F}));
}

TEST(SubWithBorrowInTest, SubtractingBorrowIn_SetsHalfBorrowAndBorrow)
{
    const auto result = SubWithBorrowIn(uint8_t{0x00}, uint8_t{0x00}, true);

    ASSERT_THAT(result.result, ::testing::Eq(uint8_t{0xFF}));
    ASSERT_THAT(result.carryBits, ::testing::Eq(uint8_t{0xFF}));
}

class SubWithBorrowTest : public ::testing::TestWithParam<SubWithBorrowTestParam>
{
};

TEST_P(SubWithBorrowTest, SubtractingTwoNumbers_ReturnsCorrectNumber)
{
    const auto& [left, right, carryBits] = GetParam();
    const auto result = SubWithBorrow(left, right);
    ASSERT_THAT(result.result, ::testing::Eq(static_cast<uint8_t>(left - right)));
}

TEST_P(SubWithBorrowTest, SubtractingTwoNumbers_ReturnsCorrectBorrowBits)
{
    const auto& [left, right, carryBits] = GetParam();
    const auto result = SubWithBorrow(left, right);
    ASSERT_THAT(result.carryBits, ::testing::Eq(carryBits));
}

INSTANTIATE_TEST_SUITE_P(
    SubWithBorrowTest,
    SubWithBorrowTest,
    ::testing::Values(
        SubWithBorrowTestParam{0u, 0u, 0b00000000},
        SubWithBorrowTestParam{255u, 0u, 0b00000000},
        SubWithBorrowTestParam{10u, 5u, 0b00000101},
        SubWithBorrowTestParam{5u, 3u, 0b00000010},
        SubWithBorrowTestParam{15u, 1u, 0b00000000},
        SubWithBorrowTestParam{127u, 1u, 0b00000000},
        SubWithBorrowTestParam{128u, 128u, 0b00000000},
        SubWithBorrowTestParam{170u, 85u, 0b01010101},
        SubWithBorrowTestParam{100u, 100u, 0b00000000},
        SubWithBorrowTestParam{0u, 1u, 0b11111111},
        SubWithBorrowTestParam{1u, 255u, 0b11111110},
        SubWithBorrowTestParam{100u, 200u, 0b10011000}));

} // namespace UtilityLib::Test
