/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/byte_utils.h"
#include "emulatorlib/cartridge.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

namespace EmulatorLib::Test
{

namespace
{

template <size_t N>
[[nodiscard]] constexpr auto CreateRomOfSize() -> std::array<std::byte, N>
{
    std::array<std::byte, N> data{0x00_b};
    for (size_t i = 0; i < N; ++i)
    {
        data[i] = static_cast<std::byte>(i);
    }
    data[0x0148] = 0x00_b;

    return data;
}


} // namespace

class CartridgeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_romPath = std::filesystem::temp_directory_path() / "test.gb";
    }

    template <size_t N>
    void WriteRomToDisk(std::array<std::byte, N> romData)
    {
        std::ofstream romFile(m_romPath, std::ios::binary);
        romFile.write(reinterpret_cast<const char *>(romData.data()), romData.size());
        romFile.close();
    }

    void TearDown() override
    {
        std::filesystem::remove(m_romPath);
    }

    std::filesystem::path m_romPath;
};

TEST_F(CartridgeTest, RomData_ReturnsReferenceToCorrectData)
{
    constexpr auto rom = CreateRomOfSize<0x8000>();
    WriteRomToDisk(rom);

    Cartridge cartridge(m_romPath);

    ASSERT_THAT(cartridge.RomData(), ::testing::ElementsAreArray(rom));
}

TEST_F(CartridgeTest, RomPath_ReturnsCorrectPath)
{
    constexpr auto rom = CreateRomOfSize<0x8000>();
    WriteRomToDisk(rom);

    Cartridge cartridge(m_romPath);

    ASSERT_THAT(cartridge.RomPath(), ::testing::Eq(m_romPath));
}

} // namespace EmulatorLib::Test