/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "utilitylib/add_with_carry.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

namespace UtilityLib::Test
{

namespace
{

struct AddWithCarryTestParam
{
    uint8_t left;
    uint8_t right;
    uint8_t carryBits;
};

} // namespace

class AddWithCarryTest : public ::testing::TestWithParam<AddWithCarryTestParam>
{
};

TEST_P(AddWithCarryTest, AddingTwoNumbers_ReturnsCorrectNumber)
{
    const auto& [left, right, carryBits] = GetParam();
    const auto result = AddWithCarry(left, right);
    ASSERT_THAT(result.result, ::testing::Eq(static_cast<uint8_t>(left + right)));
}

TEST_P(AddWithCarryTest, AddingTwoNumbers_ReturnsCorrectCarryBits)
{
    const auto& [left, right, carryBits] = GetParam();
    const auto result = AddWithCarry(left, right);
    ASSERT_THAT(result.carryBits, ::testing::Eq(carryBits));
}

INSTANTIATE_TEST_SUITE_P(AddWithCarryTest, AddWithCarryTest, ::testing::Values(AddWithCarryTestParam{0, 0, 0b00000000}, AddWithCarryTestParam{255, 0, 0b00000000}, AddWithCarryTestParam{10, 5, 0b00000000}, AddWithCarryTestParam{5, 3, 0b00000111}, AddWithCarryTestParam{15, 1, 0b00001111}, AddWithCarryTestParam{127, 1, 0b01111111}, AddWithCarryTestParam{128, 128, 0b10000000}, AddWithCarryTestParam{170, 85, 0b00000000}, AddWithCarryTestParam{100, 100, 0b01100100}, AddWithCarryTestParam{255, 1, 0b11111111}, AddWithCarryTestParam{255, 255, 0b11111111}, AddWithCarryTestParam{200, 100, 0b11000000}));

struct AddWithCarryThreeNumbersTestParam
{
    uint8_t first;
    uint8_t second;
    uint8_t third;
    uint8_t expectedResult;
    uint8_t expectedCarryBits;
};

class AddWithCarryThreeNumbersTest : public ::testing::TestWithParam<AddWithCarryThreeNumbersTestParam>
{
};

TEST_P(AddWithCarryThreeNumbersTest, AddingThreeNumbers_ReturnsCorrectNumber)
{
    const auto& [first, second, third, expectedResult, expectedCarryBits] = GetParam();
    const auto result = AddWithCarry(first, second, third);
    ASSERT_THAT(result.result, ::testing::Eq(expectedResult));
}

TEST_P(AddWithCarryThreeNumbersTest, AddingThreeNumbers_ReturnsCorrectCarryBits)
{
    const auto& [first, second, third, expectedResult, expectedCarryBits] = GetParam();
    const auto result = AddWithCarry(first, second, third);
    ASSERT_THAT(result.carryBits, ::testing::Eq(expectedCarryBits));
}

INSTANTIATE_TEST_SUITE_P(AddWithCarryThreeNumbersTest, AddWithCarryThreeNumbersTest,
                         ::testing::Values(
                             // 0 + 0 + 0 = 0, no carry
                             AddWithCarryThreeNumbersTestParam{0, 0, 0, 0, 0b00000000},
                             // 10 + 5 + 0 = 15, no carry
                             AddWithCarryThreeNumbersTestParam{10, 5, 0, 15, 0b00000000},
                             // 5 + 3 + 1 = 9, carry from lower nibble (5+3 yields carry3)
                             AddWithCarryThreeNumbersTestParam{5, 3, 1, 9, 0b00000111},
                             // 127 + 1 + 1 = 129, carry from bit 6
                             AddWithCarryThreeNumbersTestParam{127, 1, 1, 129, 0b01111111},
                             // 255 + 1 + 0 = 0 (overflow), all carry bits set
                             AddWithCarryThreeNumbersTestParam{255, 1, 0, 0, 0b11111111},
                             // 200 + 55 + 1 = 0 (256 overflows), carry bits set
                             AddWithCarryThreeNumbersTestParam{200, 55, 1, 0, 0b11111111},
                             // 100 + 100 + 1 = 201, carry from bits 6, 5 and 2
                             AddWithCarryThreeNumbersTestParam{100, 100, 1, 201, 0b01100100}));

} // namespace UtilityLib::Test