/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#pragma once

import emulatorlib.address_bus;

#include <gmock/gmock.h>

namespace EmulatorLib::Test
{

class AddressBusAddressableMock : public AddressBusAddressable
{
public:
    MOCK_METHOD(void, WriteToAddress, (uint16_t, std::byte), (override, noexcept));
    MOCK_METHOD(std::byte, ReadFromAddress, (uint16_t), (const, override, noexcept));
};

} // namespace EmulatorLib::Test