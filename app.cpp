
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
//void loadProgram(memory::Memory &memory);
void loadProgram( m6502::CPU &processor );
void showMemory(memory::Memory &mem, const hardware::Address from, const int count);

int testSize = 0;

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

    
    // Initialize memory with RESET vector
    memory.showMemory(0x0000, 0x32);
    memory.showMemory(0xFFFA, 6);
    memory.showMemory(0x2000, 0x20);
    memory.showMemory(0xFAD0, 16);

    memory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
    memory.write(0xFFFD, 0x20); //      MSB

    processor.reset();

    //    loadProgram(memory);
    loadProgram(processor);


//    processor.A(0x21);
//    processor.X(0x38);
//    processor.Y(0x10);
//    processor.S(0xF1);

//    memory.write( 0x01F1, 0x22);
//    memory.write( 0x01F2, 0x33);
    
    // std::cout << memory.name() << std::endl;
    // std::cout << processor.currentMemory().name() << std::endl;
    processor.showRegisters();

    processor.execute(testSize);

    processor.showRegisters();

    return 0;
}

void loadProgram(m6502::CPU &processor )
{
    memory::Memory memory = processor.currentMemory();
    // cpu::Address = std::to_underlying(cpu::HardwareVector::RESET);
    // int address = std::to_underlying(m6502::HardwareVector::RESET);
    // memory.write(0x2000, 0xA9); // LDA #$10
    // memory.write(0x2001, 0x21);
    // memory.write(0x2002, 0x0D); // ORA $4024
    // memory.write(0x2003, 0x24);
    // memory.write(0x2004, 0x40);

    //    memory.write(0x2000, 0xAA); // TAX
    //    memory.write(0x2000, 0x8A); // TXA
    //    memory.write(0x2000, 0xA8); // TAY
    //    memory.write(0x2000, 0x98); // TYA
    //    memory.write(0x2000, 0xCA); // DEX
    //    memory.write(0x2000, 0xE8); // INX
    //    memory.write(0x2000, 0x88); // DEY
    //    memory.write(0x2000, 0xC8); // INY

//    memory.write(0x2000, 0xC6); // DEC $72
//    memory.write(0x2001, 0x72);
//
//    memory.write(0x0072, 0x74); //

    // DEX (DEcrement X)        $CA
    // INX (INcrement X)        $E8
    // DEY (DEcrement Y)        $88
    // INY (INcrement Y)        $C8

    
//    memory.write(0x2000, 0x8C); // STY $3010
//    memory.write(0x2001, 0x10);
//    memory.write(0x2002, 0x30);
    
    //    memory.write(0x2000, 0x48); // PHA
    //    memory.write(0x2000, 0x08); // PHP
    //    memory.write(0x2000, 0x68); // PLA
    //    memory.write(0x2000, 0x28); // PLP
    //    memory.write(0x2000, 0xBA); // TSX
    //    memory.write(0x2000, 0x9A); // TXS

    
//    processor.A(0x80); // 0b1000 0000 -> 0000 0000  C:1
//    processor.clearC();

//    memory.write(0x2000, 0x2A); // ROL
//    memory.write(0x2000, 0x6A); // ROR

//    processor.A(0x22); // 0b1000 0000 -> 0000 0000  C:1
//    processor.setC();
//    memory.write(0x2000, 0x0A); // ASL

//    processor.X(0x10); // 0b1000 0000 -> 0000 0000  C:1
//    memory.write(0x2000, 0x16); // ASL
//    memory.write(0x2001, 0x08);
//    memory.write(0x0018, 0x55);

    memory.write(0x2000, 0x0E); // ASL $nnnn
    memory.write(0x2001, 0x08);
    memory.write(0x2002, 0x22);

    memory.write(0x2208, 0x22); // 0010 0010 -> 0100 0100

    

//    processor.clearC();
//    memory.write(0x2000, 0x66); // ROR $34
//    memory.write(0x2001, 0x34);
//
//    memory.write(0x0034, 0xD0);     // 0b1101 0000 -> 0110 1000 C:0

    testSize = 1;

    // memory.write(0x4024, 0x48);

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
//    memory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
//    memory.write(0xFFFD, 0x20); //      MSB

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
