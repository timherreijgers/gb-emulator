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

} // namespace UtilityLib::Test