

#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionLoadTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("LoadUnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionLoadTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionLoadTest() override
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
    //----------------------------------------
    // LDA (LoaD Accumulator)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     LDA #$44      $A9  2   2
    // Zero Page     LDA $44       $A5  2   3
    // Zero Page,X   LDA $44,X     $B5  2   4
    // Absolute      LDA $4400     $AD  3   4
    // Absolute,X    LDA $4400,X   $BD  3   4+
    // Absolute,Y    LDA $4400,Y   $B9  3   4+
    // Indirect,X    LDA ($44,X)   $A1  2   6
    // Indirect,Y    LDA ($44),Y   $B1  2   5+
    //
    // + add 1 cycle if page boundary crossed
    //----------------------------------------

    // ----- Addressing Modes -----
    // ----- Immediate #$BB
    // TODO - Need to fix this in the AddressingMode classes - the return from execute()
    //        currently it works because the addressMode execute method sets the operand, not just returns it.
    TEST_F(InstructionLoadTest, LDA_ImmediateZero)
    {
        // Destination of the reset vector - leaves zeroPage available for testing
        testMemory.write(0x2000, 0xA9); // LDA #$00
        testMemory.write(0x2001, 0x00);

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLoadTest, LDA_ImmediatePositive)
    {
        testMemory.write(0x2000, 0xA9); // LDA #$07
        testMemory.write(0x2001, 0x07);

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x07, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLoadTest, LDA_ImmediateNegative)
    {
        testMemory.write(0x2000, 0xA9); // LDA #$FF
        testMemory.write(0x2001, 0xFF);

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xFF, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionLoadTest, LDA_ZeroPageZero)
    {
        testMemory.write(0x2000, 0xA5); // LDA $0F
        testMemory.write(0x2001, 0x0F);

        testMemory.write(0x000F, 0x00); // The data value to load

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLoadTest, LDA_ZeroPagePositive)
    {
        testMemory.write(0x2000, 0xA5); // LDA $80
        testMemory.write(0x2001, 0x80);

        testMemory.write(0x0080, 0x34); // The data value to load

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x34, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLoadTest, LDA_ZeroPageNegative)
    {
        testMemory.write(0x2000, 0xA5); // LDA #$E0
        testMemory.write(0x2001, 0xE0);

        testMemory.write(0x00E0, 0xFF); // The data value to load

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xFF, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionLoadTest, LDA_ZeroPageIndexedX)
    {
        testMemory.write(0x2000, 0xA2); // LDX $02
        testMemory.write(0x2001, 0x02);
        testMemory.write(0x2002, 0xB5); // LDA $80,X
        testMemory.write(0x2003, 0x80);

        testMemory.write(0x0082, 0x64); // The data value to load

        cpu->executeFromAddress(0x2000, 2);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x64, cpu->A());
        EXPECT_EQ(0x02, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionLoadTest, LDA_Absolute)
    {
        testMemory.write(0x2000, 0xAD); // LDA $3010
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0x30);

        testMemory.write(0x3010, 0x34); // The data value to load

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x34, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionLoadTest, LDA_AbsoluteIndexedX)
    {
        testMemory.write(0x2000, 0xA2); // LDX #$12
        testMemory.write(0x2001, 0x12);
        testMemory.write(0x2002, 0xBD); // LDA $3102,X
        testMemory.write(0x2003, 0x20);
        testMemory.write(0x2004, 0x31);

        testMemory.write(0x3132, 0x78); // The data value to load

        cpu->executeFromAddress(0x2000, 2);

        EXPECT_EQ(0x2005, cpu->PC());
        EXPECT_EQ(0x78, cpu->A());
        EXPECT_EQ(0x12, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionLoadTest, LDA_AbsoluteIndexedY)
    {
        testMemory.write(0x2000, 0xA0); // LDY #$10
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0xB9); // LDA $FADE,Y
        testMemory.write(0x2003, 0xDE);
        testMemory.write(0x2004, 0xFA);

        testMemory.write(0xFAEE, 0xC3); // The data value to load

        cpu->executeFromAddress(0x2000, 2);

        EXPECT_EQ(0x2005, cpu->PC());
        EXPECT_EQ(0xC3, cpu->A());
        EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionLoadTest, LDA_IndexedIndirectX)
    {
        testMemory.write(0x2000, 0xA2); // LDX #$05
        testMemory.write(0x2001, 0x05); // Base of lookup table in page zero
        testMemory.write(0x2002, 0xA1); // LDA ($70,X)
        testMemory.write(0x2003, 0x70); // offset from base address

        // Lookup table of addresses
        testMemory.write(0x0075, 0x23); // Entry 0, $LL Address lookup table
        testMemory.write(0x0076, 0x30); //          $HH

        testMemory.write(0x3023, 0xA5); // Data

        cpu->executeFromAddress(0x2000, 2);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0xA5, cpu->A());
        EXPECT_EQ(0x05, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionLoadTest, LDA_IndirectIndexedY)
    {
        cpu->Y(0x10);
        testMemory.write(0x2002, 0xB1); // LDA ($70),Y      ; Indirect table address
        testMemory.write(0x2003, 0x70);

        // Lookup table of addresses
        testMemory.write(0x0070, 0x43); // Entry 0, $LL Address lookup table
        testMemory.write(0x0071, 0x35); //          $HH

        // Indexed address table
        testMemory.write(0x3553, 0x00); // Data
        testMemory.write(0x3554, 0x90); // Data

        testMemory.write(0x9000, 0x23); // Data

        cpu->executeFromAddress(0x2002, 1);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x23, cpu->A());
        EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    //----------------------------------------
    // LDX (LoaD X register)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     LDX #$44      $A2  2   2
    // Zero Page     LDX $44       $A6  2   3
    // Zero Page,Y   LDX $44,Y     $B6  2   4
    // Absolute      LDX $4400     $AE  3   4
    // Absolute,Y    LDX $4400,Y   $BE  3   4+
    //
    // + add 1 cycle if page boundary crossed

    // ----- Immediate #$BB
    TEST_F(InstructionLoadTest, LDX_ImmediateZero)
    {
        // Destination of the reset vector - leaves zeroPage available for testing
        testMemory.write(0x2000, 0xA2); // LDX #$00
        testMemory.write(0x2001, 0x00);

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x00, cpu->X());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLoadTest, LDX_ImmediatePositive)
    {
        // Destination of the reset vector - leaves zeroPage available for testing
        testMemory.write(0x2000, 0xA2); // LDX #$21
        testMemory.write(0x2001, 0x21);

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x21, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLoadTest, LDX_ImmediateNegative)
    {
        // Destination of the reset vector - leaves zeroPage available for testing
        testMemory.write(0x2000, 0xA2); // LDX #$F0
        testMemory.write(0x2001, 0xF0);

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF0, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionLoadTest, LDX_ZeroPage)
    {
        testMemory.write(0x2000, 0xA6); // LDX $12
        testMemory.write(0x2001, 0x12);

        testMemory.write(0x0012, 0x42);

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x42, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,X $LL,X

    // ----- ZeroPage,Y $LL,Y
    TEST_F(InstructionLoadTest, LDX_ZeroPageY)
    {
        // --- given
        testMemory.write(0x2000, 0xB6); // LDX $12,Y
        testMemory.write(0x2001, 0x12);

        cpu->X(0x99);
        cpu->Y(0x10);

        testMemory.write(0x0022, 0x24);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x24, cpu->X());
        EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionLoadTest, LDX_Absolute)
    {
        // --- given
        testMemory.write(0x2000, 0xAE); // LDX $3010
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0x30);

        testMemory.write(0x3010, 0x99);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x99, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    // ..... AbsoluteX $LLHH,X

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionLoadTest, LDX_AbsoluteY)
    {
        // --- given
        testMemory.write(0x2000, 0xBE); // LDX $3010,Y
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0x30);

        testMemory.write(0x3020, 0x68);

        cpu->Y(0x10);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x68, cpu->X());
        EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // LDY (LoaD Y register)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     LDY #$44      $A0  2   2
    // Zero Page     LDY $44       $A4  2   3
    // Zero Page,X   LDY $44,X     $B4  2   4
    // Absolute      LDY $4400     $AC  3   4
    // Absolute,X    LDY $4400,X   $BC  3   4+
    //
    // + add 1 cycle if page boundary crossed

    // ----- Immediate #$BB
    TEST_F(InstructionLoadTest, LDY_ImmediateZero)
    {
        // Destination of the reset vector - leaves zeroPage available for testing
        testMemory.write(0x2000, 0xA0); // LDY #$00
        testMemory.write(0x2001, 0x00);

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x00, cpu->Y());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLoadTest, LDY_ImmediatePositive)
    {
        // Destination of the reset vector - leaves zeroPage available for testing
        testMemory.write(0x2000, 0xA0); // LDY #$25
        testMemory.write(0x2001, 0x25);

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x25, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLoadTest, LDY_ImmediateNegative)
    {
        // Destination of the reset vector - leaves zeroPage available for testing
        testMemory.write(0x2000, 0xA0); // LDY #$F0
        testMemory.write(0x2001, 0xF0);

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF0, cpu->Y());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionLoadTest, LDY_ZeroPage)
    {
        testMemory.write(0x2000, 0xA4); // LDY $0A
        testMemory.write(0x2001, 0x0A);

        testMemory.write(0x000A, 0x66); // The data value to load

        cpu->executeFromAddress(0x2000, 1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x66, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionLoadTest, LDY_Absolute)
    {
        // --- given
        testMemory.write(0x2000, 0xAC); // LDY $3014
        testMemory.write(0x2001, 0x14);
        testMemory.write(0x2002, 0x30);

        testMemory.write(0x3014, 0x11);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x11, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionLoadTest, LDY_AbsoluteX)
    {
        // --- given
        testMemory.write(0x2000, 0xBC); // LDY $3010,X
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0x30);

        cpu->X(0x08);

        testMemory.write(0x3018, 0x71);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x08, cpu->X());
        EXPECT_EQ(0x71, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    // ----- Transfer (store) -----
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
