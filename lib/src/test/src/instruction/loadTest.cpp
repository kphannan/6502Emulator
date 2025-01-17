

#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionLoadTest : public testing::Test
    {
    public:
        // memory::Memory *tMemory = new memory::Memory("Kilroy");
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
        memory::Memory testMemory = *tMemory;
        // memory::Memory testMemory("UnitTestMemory");

        // CPU cpuX(testMemory);
        CPU *cpu;

    protected:
        InstructionLoadTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A); //

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

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionLoadTest, LDA_ImmediateZero)
    {
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xA9); // LDA #$00
        memory.write(0x2001, 0x00); //

        cpu->execute(1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100010, cpu->P());
    }

    TEST_F(InstructionLoadTest, LDA_ImmediatePositive)
    {
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xA9); // LDA #$21
        memory.write(0x2001, 0x21); //

        cpu->execute(1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x21, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
    }

    TEST_F(InstructionLoadTest, LDA_ImmediateNegative)
    {
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xA9); // LDA #$FF
        memory.write(0x2001, 0xFF); //

        cpu->execute(1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xFF, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
    }

    // ..... Implied
    // ..... Accumulator
    // ----- ZeroPage $LL
    TEST_F(InstructionLoadTest, LDA_ZeroPageZero)
    {
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xA5); // LDA $0F
        memory.write(0x2001, 0x0F); //

        memory.write(0x000F, 0x00); // The data value to load

        cpu->execute(1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100010, cpu->P());
    }

    TEST_F(InstructionLoadTest, LDA_ZeroPagePositive)
    {
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xA5); // LDA $32
        memory.write(0x2001, 0x32); //

        memory.write(0x0032, 0x27); // The data value to load

        cpu->execute(1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x27, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
    }

    TEST_F(InstructionLoadTest, LDA_ZeroPageNegative)
    {
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xA5); // LDA #$FF
        memory.write(0x2001, 0xE0); //

        memory.write(0x00E0, 0xFF); // The data value to load

        cpu->execute(1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xFF, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionLoadTest, LDA_ZeroPageIndexedX)
    {
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xA2); // LDX $10
        memory.write(0x2001, 0x10);
        memory.write(0x2002, 0xA1); // LDA $0F
        memory.write(0x2003, 0x05); //

        memory.write(0x0015, 0x72); // The data value to load

        cpu->execute(2);

        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x72, cpu->A());
        EXPECT_EQ(0x10, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB
    // ----- Absolute $LLHH
    TEST_F(InstructionLoadTest, LDA_Absolute)
    {
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xAD); // LDA $FADE
        memory.write(0x2001, 0xDE);
        memory.write(0x2002, 0xFA);

        memory.write(0xFADE, 0x62); // The data value to load

        cpu->execute(1);

        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x62, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
    }
    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionLoadTest, LDA_AbsoluteIndexedX)
    {
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xA2); // LDX #$08
        memory.write(0x2001, 0x08);
        memory.write(0x2002, 0xBD); // LDA $FADE,X
        memory.write(0x2003, 0xDE); //
        memory.write(0x2004, 0xFA); //

        memory.write(0xFAE6, 0xA9); // The data value to load

        cpu->execute(2);

        EXPECT_EQ(0x2005, cpu->PC());
        EXPECT_EQ(0xA9, cpu->A());
        EXPECT_EQ(0x08, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
    }
    // ----- AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ----- Indexed Indirect X ($LL,X)
    // ----- Indirect Indexed Y ($LL),Y

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
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xA2); // LDX #$00
        memory.write(0x2001, 0x00); //

        cpu->execute(1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x00, cpu->X());
        EXPECT_EQ(0b00100010, cpu->P());
    }

    TEST_F(InstructionLoadTest, LDX_ImmediatePositive)
    {
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xA2); // LDX #$21
        memory.write(0x2001, 0x21); //

        cpu->execute(1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x21, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
    }

    TEST_F(InstructionLoadTest, LDX_ImmediateNegative)
    {
        memory::Memory &memory = cpu->currentMemory();

        // Destination of the reset vector - leaves zeroPage available for testing
        memory.write(0x2000, 0xA2); // LDX #$F0
        memory.write(0x2001, 0xF0); //

        cpu->execute(1);

        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF0, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
    }
    // ..... Implied
    // ..... Accumulator
    // ----- ZeroPage $LL
    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB
    // ----- Absolute $LLHH
    // ..... AbsoluteX $LLHH,X
    // ----- AbsoluteY $LLHH,Y
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
    // ..... Implied
    // ..... Accumulator
    // ----- ZeroPage $LL
    // ----- ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB
    // ----- Absolute $LLHH
    // ----- AbsoluteX $LLHH,X
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
