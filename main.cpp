#include "emulatorlib/address_bus.h"
#include "emulatorlib/cartridge.h"
#include "emulatorlib/cpu.h"
#include "utilitylib/byte_utils.h"

#include <iostream>

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        std::println("Usage: emulator <rom_file>");
        std::exit(-1);
    }

    const std::string romFilePath = argv[1];

    EmulatorLib::Cartridge cartridge(romFilePath);

    EmulatorLib::AddressBus addressBus{std::vector<std::reference_wrapper<EmulatorLib::AddressBusAddressable>>{std::ref(cartridge)}};
    EmulatorLib::Cpu cpu{addressBus};

    try
    {
        while (true)
        {
            cpu.Step();
        }
    }
    catch (const std::exception& e)
    {
        std::println("Exception caught: {}", e.what());
        std::println("Register state:");
        std::println("A: {}", cpu.Registers().accumulator.value);
        std::println("F: {}", cpu.Registers().flags.value);
        std::println("B: {}", cpu.Registers().bRegister.value);
        std::println("C: {}", cpu.Registers().cRegister.value);
        std::println("D: {}", cpu.Registers().dRegister.value);
        std::println("E: {}", cpu.Registers().eRegister.value);
        std::println("H: {}", cpu.Registers().hRegister.value);
        std::println("L: {}", cpu.Registers().lRegister.value);
        std::println("SP: 0x{:04X}", cpu.Registers().stackPointer.value);
        std::println("PC: 0x{:04X}", cpu.Registers().programCounter.value);

        std::exit(-1);
    }

    return 0;
}
