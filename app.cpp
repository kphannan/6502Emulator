
#include <iostream>
#include <iomanip>
#include <stdlib.h>

// #include <cstdint>
// #include <type_traits>
// #include <utility>

// #include "app.h"
#include "memory.hpp"
#include "6502.hpp"
#include "TutorialConfig.h"

// Forward declarations
void loadProgram(memory::Memory &memory);
void showMemory(memory::Memory &mem, const hardware::Address from, const int count);

// ===== Entry Pointt =====
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        // report version
        std::cout << argv[0] << " Version " << EMU_VERSION_MAJOR << "."
                  << EMU_VERSION_MINOR << "."
                  << EMU_VERSION_PATCH << std::endl;
        std::cout << "Usage: " << argv[0] << std::endl;
        // return 1;
    }

    memory::Memory memory;
    m6502::CPU processor(memory);

    loadProgram(memory);
    processor.reset();
    memory.showMemory(0x0000, 0x32);
    memory.showMemory(0xFFFA, 6);
    memory.showMemory(0x2000, 0x20);
    memory.showMemory(0xFAD0, 16);

    // std::cout << memory.name() << std::endl;
    // std::cout << processor.currentMemory().name() << std::endl;
    processor.showRegisters();

    processor.execute(2);

    processor.showRegisters();

    return 0;
}

void loadProgram(memory::Memory &memory)
{
    // cpu::Address = std::to_underlying(cpu::HardwareVector::RESET);
    // int address = std::to_underlying(m6502::HardwareVector::RESET);

    // LDX #$08
    // LDA $FADE,X
    memory.write(0x2000, 0xA2); // LDX #$08
    memory.write(0x2001, 0x08);
    memory.write(0x2002, 0xBD); // LDA $FADE,X
    memory.write(0x2003, 0xDE); //
    memory.write(0x2004, 0xFA); //

    memory.write(0xFAE6, 0xA9); // The data value to load

    /*
    // LDA $FADE
    memory.write(0x2000, 0xAD); // LDA $FADE
    memory.write(0x2001, 0xDE);
    memory.write(0x2002, 0xFA);

    memory.write(0xFADE, 0x62); // The data value to load
    */
    /*
// LDA $05,X   x = 10  result = 72
    memory.write(0x2000, 0xA2); // LDX $10
    memory.write(0x2001, 0x10);
    memory.write(0x2002, 0xA1); // LDA $05
    memory.write(0x2003, 0x05); //

    memory.write(0x0015, 0x72); // The data value to load
    */

    // memory.write(0x0000, 0x49); // LDA #00 // starting instruction after reset
    // memory.write(0x0001, 0x55); //

    // memory.write(0x2000, 0xA5); // LDA #00 // starting instruction after reset
    // memory.write(0x2001, 0x23); //

    memory.write(0x0023, 0x42); //

    // Reset vector points to start of memory
    memory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
    memory.write(0xFFFD, 0x20); //      MSB

    /*
    memory.write(0x0000, 0x49); // LDA #00 // starting instruction after reset
    memory.write(0x0001, 0xCC); //
    memory.write(0x0002, 0x09); // ORA #A5
    memory.write(0x0003, 0xA5); //

    memory.write(0x00CC, 0x42); //
    memory.write(0xCC09, 0x62);
    memory.write(0x09CC, 0x26);

    // Reset vector points to start of memory
    memory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
    memory.write(0xFFFB, 0x00); //      MSB
    */
}

/*
    memory.write(0x0000, 0xA9); // LDA #00 // starting instruction after reset
    memory.write(0x0001, 0xCC); //
    memory.write(0x0002, 0x09); // ORA #A5
    memory.write(0x0003, 0xA5); //

    memory.write(0x00CC, 0x42); //

    // Reset vector points to start of memory
    memory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
    memory.write(0xFFFB, 0x00); //      MSB
*/
