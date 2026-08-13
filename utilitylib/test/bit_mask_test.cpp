/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "utilitylib/bit_mask.h"
#include "utilitylib/byte_utils.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

namespace UtilityLib::Test
{

class BitMaskTest : public ::testing::TestWithParam<uint8_t>
{
};

TEST_P(BitMaskTest, BitMask_ReturnsCorrectMask)
{
    ASSERT_THAT(BitMask(GetParam()), ::testing::Eq(1_b << GetParam()));
}

INSTANTIATE_TEST_SUITE_P(BitMaskTest, BitMaskTest,
                         ::testing::Values(0, 1, 2, 3, 4, 5, 6, 7));

} // namespace UtilityLib::Test