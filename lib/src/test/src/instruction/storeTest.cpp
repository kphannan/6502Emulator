

#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionStoreTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionStoreTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionStoreTest() override
        {
            delete cpu;
        }
    };

    // Addressing Modes
    // ----- Immediate #$BB
    // ----- Implied
    // ----- Accumulator
    // ----- ZeroPage $LL
    // ----- ZeroPage,X $LL,X
    // ----- ZeroPage,Y $LL,Y
    // ----- Relative $BB
    // ----- Absolute $LLHH
    // ----- AbsoluteX $LLHH,X
    // ----- AbsoluteY $LLHH,Y
    // ----- Indirect ($LLHH)
    // ----- Indexed Indirect X ($LL,X)
    // ----- Indirect Indexed Y ($LL),Y

    // ========== Instructions ==========

    // ----- Transfer (load) -----
    // ----- Transfer (store) -----
    //----------------------------------------
    // STA (STore Accumulator)
    //
    // Affects Flags: none
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     STA $44       $85  2   3
    // Zero Page,X   STA $44,X     $95  2   4
    // Absolute      STA $4400     $8D  3   4
    // Absolute,X    STA $4400,X   $9D  3   5
    // Absolute,Y    STA $4400,Y   $99  3   5
    // Indirect,X    STA ($44,X)   $81  2   6
    // Indirect,Y    STA ($44),Y   $91  2   6
    //----------------------------------------

    // ----- Addressing Modes -----
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator
    // ----- ZeroPage $LL
    TEST_F(InstructionStoreTest, STA_ZeroPageZero)
    {
        testMemory.write(0x2002, 0x85); // STA #$08
        testMemory.write(0x2003, 0x08);
        cpu->A(0x00);

        // Hard set a wrong value
        testMemory.write(0x0007, 0x44); // The data value to load
        testMemory.write(0x0008, 0x11); // The data value to load
        testMemory.write(0x0009, 0x44); // The data value to load

        cpu->executeFromAddress(0x2002, 1);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(0x00, testMemory.read(0x0008));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
    }

    TEST_F(InstructionStoreTest, STA_ZeroPagePositive)
    {
        // testMemory.write(0x2000, 0xA5); // LDA $80
        // testMemory.write(0x2001, 0x80);

        // testMemory.write(0x2000, 0xA9); // LDA #$0F
        // testMemory.write(0x2001, 0x34);
        testMemory.write(0x2002, 0x85); // STA #$08
        testMemory.write(0x2003, 0x10);
        cpu->A(0x33);

        cpu->executeFromAddress(0x2002, 1);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x33, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x33, testMemory.read(0x0010));
        // EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        // EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
    }

    TEST_F(InstructionStoreTest, STA_ZeroPageNegative)
    {
        testMemory.write(0x2000, 0x85); // STA #$E0
        testMemory.write(0x2001, 0xE0);
        cpu->A(0xFF);

        testMemory.write(0x00E0, 0x99); // The data value to load

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xFF, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0xFF, testMemory.read(0x00E0));
        // EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        // EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionStoreTest, STA_ZeroPageIndexedX)
    {
        testMemory.write(0x2000, 0xA2); // LDX $02
        testMemory.write(0x2001, 0x02);
        testMemory.write(0x2002, 0x95); // STA $80,X
        testMemory.write(0x2003, 0x80);
        cpu->A(0x64);

        testMemory.write(0x0082, 0x55); // put something wrong at the destination

        cpu->executeFromAddress(0x2000, 2);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x64, cpu->A());
        EXPECT_EQ(0x02, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x64, testMemory.read(0x0082));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB
    // ----- Absolute $LLHH
    TEST_F(InstructionStoreTest, STA_Absolute)
    {
        testMemory.write(0x2000, 0x8D); // STA $3010
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0x30);
        cpu->A(0x64);

        testMemory.write(0x3010, 0x34); // Known bad non-zero value

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x64, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x64, testMemory.read(0x3010));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionStoreTest, STA_AbsoluteIndexedX)
    {
        testMemory.write(0x2000, 0xA2); // LDX #$12
        testMemory.write(0x2001, 0x12);
        testMemory.write(0x2002, 0x9D); // STA $3102,X
        testMemory.write(0x2003, 0x20);
        testMemory.write(0x2004, 0x31);
        cpu->A(0x78);

        testMemory.write(0x3132, 0x44); // Known bad non-zero value

        cpu->executeFromAddress(0x2000, 2);

        EXPECT_EQ(0x2005, cpu->PC());
        EXPECT_EQ(0x78, cpu->A());
        EXPECT_EQ(0x12, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x78, testMemory.read(0x3132));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
    }
    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionStoreTest, STA_AbsoluteIndexedY)
    {
        testMemory.write(0x2000, 0xA0); // LDY #$10
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0x99); // STA $FADE,Y
        testMemory.write(0x2003, 0xDE);
        testMemory.write(0x2004, 0xFA);
        cpu->A(0xC3);

        testMemory.write(0xFAEE, 0xCC); // Known bad non-zero value

        cpu->executeFromAddress(0x2000, 2);

        EXPECT_EQ(0x2005, cpu->PC());
        EXPECT_EQ(0xC3, cpu->A());
        EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0xC3, testMemory.read(0xFAEE));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
    }

    // ..... Indirect ($LLHH)
    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionStoreTest, STA_IndexedIndirectX)
    {
        testMemory.write(0x2000, 0xA2); // LDX #$05
        testMemory.write(0x2001, 0x05); // Base of lookup table in page zero
        testMemory.write(0x2002, 0x81); // STA ($70,X)
        testMemory.write(0x2003, 0x70); // offset from base address
        cpu->A(0xA5);

        // Lookup table of addresses
        testMemory.write(0x0075, 0x23); // Entry 0, $LL Address lookup table
        testMemory.write(0x0076, 0x30); //          $HH

        testMemory.write(0x3023, 0xA4); // Data

        cpu->executeFromAddress(0x2000, 2);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0xA5, cpu->A());
        EXPECT_EQ(0x05, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0xA5, testMemory.read(0x3023));

        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
    }
    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionStoreTest, STA_IndexedIndirectY)
    {
        testMemory.write(0x2000, 0xA0); // LDY #$10         ; Offset into table
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0x91); // STA ($70),Y      ; Indirect table address
        testMemory.write(0x2003, 0x70);
        cpu->A(0x23);

        // Lookup table of addresses
        testMemory.write(0x0070, 0x43); // Entry 0, $LL Address lookup table
        testMemory.write(0x0071, 0x35); //          $HH

        testMemory.write(0x3553, 0x25); // Data

        cpu->executeFromAddress(0x2000, 2);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x23, cpu->A());
        EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x23, testMemory.read(0x3553));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
    }

    //----------------------------------------
    // STX (STore X register)
    //
    // Affects Flags: none
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     STX $44       $86  2   3
    // Zero Page,Y   STX $44,Y     $96  2   4
    // Absolute      STX $4400     $8E  3   4
    //----------------------------------------

    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator
    // ----- ZeroPage $LL
    TEST_F(InstructionStoreTest, STX_ZeroPageZero)
    {
        testMemory.write(0x2002, 0x86); // STX #$08
        testMemory.write(0x2003, 0x08);
        cpu->X(0x89);

        // Hard set a wrong value
        testMemory.write(0x0007, 0x44); // The data value to load
        testMemory.write(0x0008, 0x11); // The data value to load
        testMemory.write(0x0009, 0x44); // The data value to load

        cpu->executeFromAddress(0x2002, 1);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x89, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0x89, testMemory.read(0x0008));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
    }
    // ..... ZeroPage,X $LL,X
    // ----- ZeroPage,Y $LL,Y
    TEST_F(InstructionStoreTest, STX_ZeroPageY)
    {
        testMemory.write(0x2002, 0x96); // STX $08,Y
        testMemory.write(0x2003, 0x08);
        cpu->X(0x38);
        cpu->Y(0x10);

        // Hard set a wrong value
        testMemory.write(0x0007, 0x44); // The data value to load
        testMemory.write(0x0008, 0x11); // The data value to load
        testMemory.write(0x0009, 0x44); // The data value to load

        cpu->executeFromAddress(0x2002, 1);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x38, cpu->X());
        EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x38, testMemory.read(0x0018));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
    }
    // ..... Relative $BB
    // ----- Absolute $LLHH
    TEST_F(InstructionStoreTest, STX_Absolute)
    {
        testMemory.write(0x2000, 0x8E); // STX $3010
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0x30);
        cpu->X(0x62);

        testMemory.write(0x3010, 0x34); // Known bad non-zero value

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x62, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x62, testMemory.read(0x3010));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
    }
    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // STY (STore Y register)
    //
    // Affects Flags: none
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     STY $44       $84  2   3
    // Zero Page,X   STY $44,X     $94  2   4
    // Absolute      STY $4400     $8C  3   4
    //----------------------------------------

    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator
    // ----- ZeroPage $LL
    TEST_F(InstructionStoreTest, STY_ZeroPageZero)
    {
        testMemory.write(0x2002, 0x84); // STY #$08
        testMemory.write(0x2003, 0x08);
        cpu->Y(0x89);

        // Hard set a wrong value
        testMemory.write(0x0007, 0x44); // The data value to load
        testMemory.write(0x0008, 0x11); // The data value to load
        testMemory.write(0x0009, 0x44); // The data value to load

        cpu->executeFromAddress(0x2002, 1);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x89, cpu->Y());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0x89, testMemory.read(0x0008));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().src);
    }
    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionStoreTest, STY_ZeroPageX)
    {
        testMemory.write(0x2002, 0x94); // STY $08,X
        testMemory.write(0x2003, 0x04);
        cpu->X(0x14);
        cpu->Y(0x10);

        // Hard set a wrong value
        testMemory.write(0x0007, 0x44); // The data value to load
        testMemory.write(0x0008, 0x11); // The data value to load
        testMemory.write(0x0009, 0x44); // The data value to load

        cpu->executeFromAddress(0x2002, 1);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x14, cpu->X());
        EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x10, testMemory.read(0x0018));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().src);
    }
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB
    // ----- Absolute $LLHH
    TEST_F(InstructionStoreTest, STY_Absolute)
    {
        testMemory.write(0x2000, 0x8C); // STY $3010
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0x30);
        cpu->Y(0x64);

        testMemory.write(0x3010, 0x34); // Known bad non-zero value

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x64, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x64, testMemory.read(0x3010));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().src);
    }
    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    // ----- Transfer (interregister transfer) -----

    // ----- Stack () -----
    // ----- Decrement & Increment () -----
    // ----- Arithmetic () -----
    // ----- Logical () -----
    // ----- Shift & Rotate () -----
    // ----- Flag () -----
    // ----- Comparison () -----
    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----

}
