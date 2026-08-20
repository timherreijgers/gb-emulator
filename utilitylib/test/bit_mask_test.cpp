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

static_assert(BitMask<0> == 1_b);
static_assert(BitMask<1> == 2_b);
static_assert(BitMask<2> == 4_b);
static_assert(BitMask<3> == 8_b);
static_assert(BitMask<4> == 16_b);
static_assert(BitMask<5> == 32_b);
static_assert(BitMask<6> == 64_b);
static_assert(BitMask<7> == 128_b);

} // namespace UtilityLib::Test