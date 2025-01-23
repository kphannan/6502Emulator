

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionLogicalTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionLogicalTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionLogicalTest() override
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
    // ----- Transfer (interregister transfer) -----

    // ----- Stack () -----
    // ----- Decrement & Increment () -----
    // ----- Arithmetic () -----
    // ----- Logical () -----

    //----------------------------------------
    // ORA (bitwise OR with Accumulator)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     ORA #$44      $09  2   2
    // Zero Page     ORA $44       $05  2   3
    // Zero Page,X   ORA $44,X     $15  2   4
    // Absolute      ORA $4400     $0D  3   4
    // Absolute,X    ORA $4400,X   $1D  3   4+
    // Absolute,Y    ORA $4400,Y   $19  3   4+
    // Indirect,X    ORA ($44,X)   $01  2   6
    // Indirect,Y    ORA ($44),Y   $11  2   5+
    //
    // + add 1 cycle if page boundary crossed
    //----------------------------------------
    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionLogicalTest, ORA_Immediate)
    {
        // --- given
        cpu->A(0x11);
        testMemory.write(0x2000, 0x09); // ORA #$22
        testMemory.write(0x2001, 0x22);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x33, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionLogicalTest, ORA_ZeroPage)
    {
        // --- given
        cpu->A(0x21);
        testMemory.write(0x2000, 0x05); // ORA $04
        testMemory.write(0x2001, 0x04);

        testMemory.write(0x0004, 0x26);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x27, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionLogicalTest, ORA_ZeroPageX)
    {
        // --- given
        cpu->A(0x84);
        cpu->X(0x05);
        testMemory.write(0x2000, 0x15); // ORA $04,X
        testMemory.write(0x2001, 0x04);

        testMemory.write(0x0009, 0x71);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF5, cpu->A());
        EXPECT_EQ(0x05, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionLogicalTest, ORA_Absolute)
    {
        // --- given
        cpu->A(0x21);
        testMemory.write(0x2000, 0x0D); // ORA $4024
        testMemory.write(0x2001, 0x24);
        testMemory.write(0x2002, 0x40);

        testMemory.write(0x4024, 0x48);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x69, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionLogicalTest, ORA_AbsoluteX)
    {
        // --- given
        cpu->A(0x08);
        cpu->X(0x08);
        testMemory.write(0x2000, 0x1D); // ORA $3010,X
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0x30);

        testMemory.write(0x3018, 0x80);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x88, cpu->A());
        EXPECT_EQ(0x08, cpu->X());
        // EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionLogicalTest, ORA_AbsoluteY)
    {
        // --- given
        cpu->A(0x07);
        cpu->Y(0x04);
        testMemory.write(0x2000, 0x19); // ORA $3012,Y
        testMemory.write(0x2001, 0x12);
        testMemory.write(0x2002, 0x30);

        testMemory.write(0x3016, 0x70);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x77, cpu->A());
        EXPECT_EQ(0x04, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionLogicalTest, ORA_IndirectX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionLogicalTest, ORA_IndirectY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- Shift & Rotate () -----
    // ----- Flag () -----
    // ----- Comparison () -----
    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
