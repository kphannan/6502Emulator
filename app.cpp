
#include <iostream>
#include <iomanip>
#include <stdlib.h>

// #include <cstdint>
// #include <type_traits>
// #include <utility>

// #include "app.h"
#include "memory.hpp"
#include "6502.hpp"
// #include "TutorialConfig.h"

// Forward declarations
void loadProgram(memory::Memory &memory);
void showMemory(memory::Memory &mem, const hardware::Address from, const int count);

// ===== Entry Pointt =====
int main(int argc, char **argv)
{
    // if (argc < 2)
    // {
    //     // report version
    //     std::cout << argv[0] << " Version " << EMU_VERSION_MAJOR << "."
    //               << EMU_VERSION_MINOR << "."
    //               << EMU_VERSION_PATCH << std::endl;
    //     std::cout << "Usage: " << argv[0] << std::endl;
    //     // return 1;
    // }

    memory::Memory memory;
    m6502::CPU processor(memory);

    loadProgram(memory);
    processor.reset();
    memory.showMemory(0x0000, 0x32);
    memory.showMemory(0xFFFA, 6);
    memory.showMemory(0x2000, 0x20);
    memory.showMemory(0xFAD0, 16);

    processor.A(0x21);
    processor.X(0x38);
    processor.Y(0x10);

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
    memory.write(0x2000, 0xA9); // LDA #$10
    memory.write(0x2001, 0x21);
    memory.write(0x2002, 0x0D); // ORA $4024
    memory.write(0x2003, 0x24);
    memory.write(0x2004, 0x40);

    memory.write(0x4024, 0x48);

    // memory.write(0x2000, 0xA9); // LDA #$10
    // memory.write(0x2001, 0x10);
    // memory.write(0x2002, 0xA0); // LDY #$10
    // memory.write(0x2003, 0x10);
    // memory.write(0x2004, 0x99); // STA $FADE,Y
    // memory.write(0x2005, 0xDE);
    // memory.write(0x2006, 0xFA);
    // testMemory.write(0x000F, 0x00); // The data value to load

    // cpu->A(0x32);
    // cpu->executeFromAddress(0x2002, 1);

    // memory.write(0x2000, 0xA0); // LDY #$10         ; Offset into table
    // memory.write(0x2001, 0x10); //
    // memory.write(0x2002, 0xB1); // LDA ($70),Y      ; Indirect table address
    // memory.write(0x2003, 0x70); //

    // // Lookup table of addresses
    // memory.write(0x0070, 0x43); // Entry 0, $LL Address lookup table
    // memory.write(0x0071, 0x35); //          $HH

    // memory.write(0x3553, 0x23); // Data
    /*
    // LDA ($20,X)
    memory.write(0x2000, 0xA2); // LDX #$10
    memory.write(0x2001, 0x10);
    memory.write(0x2002, 0xA1); // LDA ($nn,X)
    memory.write(0x2003, 0x20); //
    // memory.write(0x2004, 0xFA); //

    memory.write(0x0030, 0xEF); // The data value to load

    memory.write(0x0020, 0xEF); // Indirect address LL
    memory.write(0x0021, 0xBE); // HH

    memory.write(0xBEFF, 0xC3); // The data value to load
    */
    /*
    // LDX #$08
    // LDA $FADE,X
    memory.write(0x2000, 0xA0); // LDX #$08
    memory.write(0x2001, 0x08);
    memory.write(0x2002, 0xB9); // LDA $FADE,Y
    memory.write(0x2003, 0xDE); //
    memory.write(0x2004, 0xFA); //

    memory.write(0xFAE6, 0xA9); // The data value to load
    */
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

    // memory.write(0x0023, 0x42); //

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
