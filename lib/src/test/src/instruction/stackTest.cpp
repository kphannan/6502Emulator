

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionStackTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("StackUnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionStackTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionStackTest() override
        {
            delete cpu;
        }
    };

    // ========== Instructions ==========

    // ----- Transfer (load) -----
    // ----- Transfer (store) -----
    // ----- Transfer (interregister transfer) -----
    // ----- Stack () -----

    //----------------------------------------
    // Stack Instructions
    //
    // These instructions are implied mode, have a length of one byte and require machine cycles as indicated. The "PuLl"
    // operations are known as "POP" on most other microprocessors. With the 6502, the stack is always on page one ($100-$1FF) and
    // works top down.
    //
    // MNEMONIC                        HEX TIM
    // TXS (Transfer X to Stack ptr)   $9A  2
    // TSX (Transfer Stack ptr to X)   $BA  2
    // PHA (PusH Accumulator)          $48  3
    // PLA (PuLl Accumulator)          $68  4
    // PHP (PusH Processor status)     $08  3
    // PLP (PuLl Processor status)     $28  4
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB

    // ----- Implied
    TEST_F(InstructionStackTest, TXS)
    {
        // --- given
        cpu->X(0xF0);
        cpu->S(0x99);
        testMemory.write(0x2000, 0x9A); // TXS
        // testMemory.write(0x2001, 0x31);
        // testMemory.write(0x2002, 0x72);

        // testMemory.write(0x7235, 0x08); //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0xF0, cpu->X());
        EXPECT_EQ(0xF0, cpu->S());
        EXPECT_EQ(0b10100000, cpu->P());
        // EXPECT_EQ(0x09, testMemory.read(0x7235));
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::S, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionStackTest, TSX) // TODO add cases for sp <,= 0
    {
        // --- given
        cpu->X(0xF0);
        cpu->S(0x99);
        testMemory.write(0x2000, 0xBA); // TXS

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x99, cpu->X());
        EXPECT_EQ(0x99, cpu->S());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::S, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionStackTest, PHA_Implied)
    {
        // --- given
        cpu->A(0x3A);
        cpu->S(0xF7);
        testMemory.write(0x2000, 0x48); // PHA

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x3A, cpu->A());
        EXPECT_EQ(0xF6, cpu->S());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x3A, testMemory.read(0x01F7));
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::STACK, cpu->decodePipeline().dst);
        //        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    TEST_F(InstructionStackTest, PLA_Implied)
    {
        // --- given
        cpu->A(0x3A);
        cpu->S(0xF6);
        testMemory.write(0x2000, 0x68); // PLA

        testMemory.write(0x01F7, 0xCE);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0xCE, cpu->A());
        EXPECT_EQ(0xF7, cpu->S());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0xCE, testMemory.read(0x01F7));
        EXPECT_EQ(InstructionTarget::STACK, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionStackTest, PHP_Implied)
    {
        // --- given
        cpu->P(0xFF);
        cpu->S(0xF2);
        testMemory.write(0x2000, 0x08); // PHP

        testMemory.write(0x01F2, 0x33);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0xFF, cpu->P());
        EXPECT_EQ(0xF1, cpu->S());
        EXPECT_EQ(0b11111111, cpu->P());
        EXPECT_EQ(0xFF, testMemory.read(0x01F2));
        EXPECT_EQ(InstructionTarget::PSR, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::STACK, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionStackTest, PLP_Implied)
    {
        // --- given
        cpu->P(0xFF);
        cpu->S(0xF1);
        testMemory.write(0x2000, 0x28); // PLP

        testMemory.write(0x01F2, 0x33);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x33, cpu->P());
        EXPECT_EQ(0xF2, cpu->S());
        EXPECT_EQ(0b00110011, cpu->P());
        EXPECT_EQ(0x33, testMemory.read(0x01F2));
        EXPECT_EQ(InstructionTarget::STACK, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PSR, cpu->decodePipeline().dst);
    }

    // ..... Accumulator
    // ..... ZeroPage $LL
    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB
    // ..... Absolute $LLHH
    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

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
