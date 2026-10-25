/*
 * Copyright © 2026. Tim Herreijgers
 * Licensed using the MIT license
 */

#include "emulatorlib/cartridge.h"
#include "utilitylib/byte_utils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <thread>

namespace EmulatorLib::Test
{

namespace
{

constexpr std::array g_nintendoLogoData = {
    0xCE_b, 0xED_b, 0x66_b, 0x66_b,
    0xCC_b, 0x0D_b, 0x00_b, 0x0B_b,
    0x03_b, 0x73_b, 0x00_b, 0x83_b,
    0x00_b, 0x0C_b, 0x00_b, 0x0D_b,
    0x00_b, 0x08_b, 0x11_b, 0x1F_b,
    0x88_b, 0x89_b, 0x00_b, 0x0E_b,
    0xDC_b, 0xCC_b, 0x6E_b, 0xE6_b,
    0xDD_b, 0xDD_b, 0xD9_b, 0x99_b,
    0xBB_b, 0xBB_b, 0x67_b, 0x63_b,
    0x6E_b, 0x0E_b, 0xEC_b, 0xCC_b,
    0xDD_b, 0xDC_b, 0x99_b, 0x9F_b,
    0xBB_b, 0xB9_b, 0x33_b, 0x3E_b};

constexpr auto g_headers = std::array{
    0x00_b,
    0x01_b,
    0x02_b,
    0x03_b,
    0x00_b,
    0x01_b,
    0x02_b,
    0x03_b,
    0x00_b,
    0x01_b,
    0x02_b,
    0x03_b,
    0x00_b,
    0x01_b,
    0x02_b,
    0x03_b,
    0x00_b,
    0x01_b,
    0x02_b,
    0x03_b,
    0x00_b,
    0x01_b,
    0x02_b,
    0x03_b,
    0x04_b,
};

template <size_t N>
[[nodiscard]] constexpr auto CreateEmptyRomOfSize() -> std::array<std::byte, N>
{
    return std::array<std::byte, N>{0x00_b};
}

template <size_t N>
[[nodiscard]] constexpr auto CreateEmptyVectorRomOfSize() -> std::vector<std::byte>
{
    return std::vector(N, 0x00_b);
}

template <size_t RomSize, size_t DataSize>
[[nodiscard]] constexpr auto EmplaceDataInRom(std::array<std::byte, RomSize> romData, std::array<std::byte, DataSize> data, size_t startAddress) -> std::array<std::byte, RomSize>
{
    for (size_t i = 0; i < DataSize; ++i)
    {
        romData[startAddress + i] = data[i];
    }
    return romData;
}

template <size_t RomSize>
[[nodiscard]] constexpr auto EmplaceDataInRom(std::array<std::byte, RomSize> romData, std::string_view data, size_t startAddress) -> std::array<std::byte, RomSize>
{
    for (size_t i = 0; i < data.size(); ++i)
    {
        romData[startAddress + i] = static_cast<std::byte>(data[i]);
    }
    return romData;
}

template <size_t RomSize>
[[nodiscard]] constexpr auto EmplaceDataInRom(std::array<std::byte, RomSize> romData, std::byte data, size_t startAddress) -> std::array<std::byte, RomSize>
{
    romData[startAddress] = data;
    return romData;
}

template <size_t Size>
constexpr auto CalculateChecksum(std::array<std::byte, Size> data) -> std::byte
{
    auto checksum = 0x00_b;
    for (size_t i = 0; i < Size; ++i)
    {
        checksum = checksum - data[i] - 1_b;
    }
    return checksum;
}

struct CartridgeHeaderRomSizePair
{
    std::byte romSizeHeaderByte;
    bool shouldThrow;
};

[[nodiscard]] constexpr auto GenerateCartridgeHeaderRomSizePairs() noexcept -> std::array<CartridgeHeaderRomSizePair, 0x100>
{
    std::array<CartridgeHeaderRomSizePair, 0x100> pairs{};

    for (size_t i = 0; i <= 0xFF; ++i)
    {
        pairs[i] = {static_cast<std::byte>(i), i > 0x08};
    }

    return pairs;
}

} // namespace

class CartridgeHeaderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_romPath = std::filesystem::temp_directory_path() / std::format("cartridge-header-test-{}.gb",
                                                                         std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())));
    }

    template <size_t N>
    void WriteRomToDisk(const std::array<std::byte, N>& romData)
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

TEST_F(CartridgeHeaderTest, HasValidNintendoLogo_WithoutValidNintendoLogo_ReturnsFalse)
{
    constexpr auto rom = CreateEmptyRomOfSize<0x8000>();
    WriteRomToDisk(rom);

    Cartridge cartridge(m_romPath);

    ASSERT_THAT(cartridge.HasValidNintendoLogo(), ::testing::Eq(false));
}

TEST_F(CartridgeHeaderTest, HasValidNintendoLogo_WithValidNintendoLogo_ReturnsTrue)
{
    constexpr auto rom = EmplaceDataInRom(CreateEmptyRomOfSize<0x8000>(), g_nintendoLogoData, 0x0104);
    WriteRomToDisk(rom);

    Cartridge cartridge(m_romPath);

    ASSERT_THAT(cartridge.HasValidNintendoLogo(), ::testing::Eq(true));
}

TEST_F(CartridgeHeaderTest, Title_WithTitleFullyFilled_ReturnsCorrectTitle)
{
    constexpr auto rom = EmplaceDataInRom(CreateEmptyRomOfSize<0x8000>(), "THISISAFULLTITLE", 0x0134);
    WriteRomToDisk(rom);

    Cartridge cartridge(m_romPath);

    ASSERT_THAT(cartridge.Title(), ::testing::Eq("THISISAFULLTITLE"));
}


TEST_F(CartridgeHeaderTest, Title_WithTitlePartiallyFilled_ReturnsCorrectTitlePaddedWithZeros)
{
    constexpr auto rom = EmplaceDataInRom(CreateEmptyRomOfSize<0x8000>(), "SMALL TITLE", 0x0134);
    WriteRomToDisk(rom);

    Cartridge cartridge(m_romPath);

    ASSERT_THAT(cartridge.Title(), ::testing::Eq(std::string_view{"SMALL TITLE\0\0\0\0\0", 16}));
}

TEST_F(CartridgeHeaderTest, RomSize_WithSizeSetTo32Kb_ReturnsCorrectValue)
{
    constexpr auto rom = CreateEmptyRomOfSize<0x8000>();
    WriteRomToDisk(rom);

    Cartridge cartridge(m_romPath);

    ASSERT_THAT(cartridge.RomSize(), ::testing::Eq(0x8000));
}

TEST_F(CartridgeHeaderTest, RomSize_WithSizeSetTo64Kb_ReturnsCorrectValue)
{
    constexpr auto rom = EmplaceDataInRom(CreateEmptyRomOfSize<0x10000>(), 0x01_b, 0x0148);
    WriteRomToDisk(rom);

    Cartridge cartridge(m_romPath);

    ASSERT_THAT(cartridge.RomSize(), ::testing::Eq(0x10000));
}

TEST_F(CartridgeHeaderTest, ValidHeaderChecksum_WithValidChecksum_ReturnsTrue)
{
    constexpr auto expectedChecksum = CalculateChecksum(g_headers);
    constexpr auto rom = EmplaceDataInRom(
        EmplaceDataInRom(CreateEmptyRomOfSize<0x8000>(), g_headers, 0x0134),
        expectedChecksum, 0x014D);

    WriteRomToDisk(rom);
    Cartridge cartridge(m_romPath);

    ASSERT_THAT(cartridge.ValidHeaderChecksum(), ::testing::Eq(true));
}

TEST_F(CartridgeHeaderTest, ValidHeaderChecksum_WithInvalidChecksum_ReturnsFalse)
{
    constexpr auto expectedChecksum = CalculateChecksum(g_headers);
    constexpr auto rom = EmplaceDataInRom(
        EmplaceDataInRom(CreateEmptyRomOfSize<0x8000>(), g_headers, 0x0134),
        expectedChecksum + 1, 0x014D);

    WriteRomToDisk(rom);
    Cartridge cartridge(m_romPath);

    ASSERT_THAT(cartridge.ValidHeaderChecksum(), ::testing::Eq(false));
}

TEST_F(CartridgeHeaderTest, CartridgeCreation_WithMismatchedSizeInHeader_Throws)
{
    constexpr auto rom =
        EmplaceDataInRom(CreateEmptyRomOfSize<0x8000>(), 0x01_b, 0x0148);
    WriteRomToDisk(rom);

    ASSERT_THROW(Cartridge{m_romPath}, MismatchedRomSizeException);
}

TEST_F(CartridgeHeaderTest, CartridgeCreation_WithRomTooSmallForHeader_Throws)
{
    constexpr auto rom = CreateEmptyRomOfSize<0x14F>();
    WriteRomToDisk(rom);

    ASSERT_THROW(Cartridge{m_romPath}, InvalidRomException);
}

class CartridgeHeaderCartridgeTypeTest : public CartridgeHeaderTest, public ::testing::WithParamInterface<CartridgeType>
{
};

TEST_P(CartridgeHeaderCartridgeTypeTest, CartridgeType_ReturnsCorrectCartridgeType)
{
    const auto cartridgeType = GetParam();

    const auto rom = EmplaceDataInRom(CreateEmptyRomOfSize<0x8000>(), std::array{static_cast<std::byte>(cartridgeType)}, 0x0147);
    WriteRomToDisk(rom);

    Cartridge cartridge(m_romPath);

    ASSERT_THAT(cartridge.CartridgeType(), ::testing::Eq(cartridgeType));
}

INSTANTIATE_TEST_SUITE_P(CartridgeHeaderCartridgeTypeTest, CartridgeHeaderCartridgeTypeTest,
                         ::testing::Values(
                             CartridgeType::ROM_ONLY,
                             CartridgeType::MBC1,
                             CartridgeType::MBC1_RAM,
                             CartridgeType::MBC1_RAM_BATTERY,
                             CartridgeType::MBC2,
                             CartridgeType::MBC2_BATTERY,
                             CartridgeType::ROM_RAM,
                             CartridgeType::ROM_RAM_BATTERY,
                             CartridgeType::MMM01,
                             CartridgeType::MMM01_RAM,
                             CartridgeType::MMM01_RAM_BATTERY,
                             CartridgeType::MBC3_TIMER_BATTERY,
                             CartridgeType::MBC3_TIMER_RAM_BATTERY,
                             CartridgeType::MBC3,
                             CartridgeType::MBC3_RAM,
                             CartridgeType::MBC3_RAM_BATTERY,
                             CartridgeType::MBC5,
                             CartridgeType::MBC5_RAM,
                             CartridgeType::MBC5_RAM_BATTERY,
                             CartridgeType::MBC5_RUMBLE,
                             CartridgeType::MBC5_RUMBLE_RAM,
                             CartridgeType::MBC5_RUMBLE_RAM_BATTERY,
                             CartridgeType::MBC6,
                             CartridgeType::MBC7_SENSOR_RUMBLE_RAM_BATTERY,
                             CartridgeType::POCKET_CAMERA,
                             CartridgeType::BANDAI_TAMA5,
                             CartridgeType::HUC3,
                             CartridgeType::HUC1_RAM_BATTERY));

class CartridgeHeaderRomSizeTest : public CartridgeHeaderTest, public ::testing::WithParamInterface<CartridgeHeaderRomSizePair>
{
};

TEST_P(CartridgeHeaderRomSizeTest, Test)
{
    const auto [romSizeHeaderByte, shouldThrow] = GetParam();
    const auto rom = EmplaceDataInRom(CreateEmptyRomOfSize<0x200>(), romSizeHeaderByte, 0x0148);
    WriteRomToDisk(rom);

    if (shouldThrow)
    {
        ASSERT_THROW(Cartridge{CartridgeHeaderTest::m_romPath}, InvalidRomException);
    }
    else
    {
        // We're always throwing due to the fact we can't allocate std::arrays of 8MB on the stack. If we get the
        // MismatchedRomSizeException we've already validated that the ROM size byte in the ROM header is valid.
        ASSERT_THROW(Cartridge{CartridgeHeaderTest::m_romPath}, MismatchedRomSizeException);
    }
}

INSTANTIATE_TEST_SUITE_P(CartridgeHeaderRomSizeTest, CartridgeHeaderRomSizeTest, ::testing::ValuesIn(GenerateCartridgeHeaderRomSizePairs()));

} // namespace EmulatorLib::Test