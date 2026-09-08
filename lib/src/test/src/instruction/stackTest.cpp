

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
            testMemory[0x2000] = 0x49; // LDA #00 // starting instruction after reset
            testMemory[0x2001] = 0x5A;

            // Reset vector points to start of memory
            testMemory[0xFFFC] = 0x00; // LSB cpu::HardwareVector::RESET
            testMemory[0xFFFD] = 0x20; // MSB

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
        cpu->X(0x70);
        cpu->S(0x99);
        testMemory[0x2000] = 0x9A; // TXS - X to Transfer Stack Pointer
                                   //        cpu->P( 0b00100000 );  // reset to default after setting X

        // A: ...
        // X: 0xFO ->-+
        // Y: ...     |
        // S: 0x99 -<-+  (no flags)
        // P: ...

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x70, cpu->X());
        EXPECT_EQ(0x0170, cpu->S());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::S, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionStackTest, TSX_zero) // TODO add cases for sp <,= 0
    {
        // --- given
        cpu->X(0x10);
        cpu->S(0x00);
        testMemory[0x2000] = 0xBA; // TXS

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x00, cpu->X());
        EXPECT_EQ(0x0100, cpu->S());
        EXPECT_EQ(0b00100010, cpu->P());

        EXPECT_EQ(InstructionTarget::S, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionStackTest, TSX_negative) // TODO add cases for sp <,= 0
    {
        // --- given
        cpu->X(0xF0);
        cpu->S(0x80);
        testMemory[0x2000] = 0xBA; // TXS

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x80, cpu->X());
        EXPECT_EQ(0x0180, cpu->S());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::S, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionStackTest, PHA_Implied)
    {
        // --- given
        cpu->A(0x3A);
        cpu->S(0xF7);
        testMemory[0x2000] = 0x48; // PHA - Push Accumulator

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x3A, cpu->A());
        EXPECT_EQ(0x01F6, cpu->S());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x3A, testMemory.read(0x01F7));
        EXPECT_EQ(0x3A, testMemory[0x01F7]);

        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::STACK, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionStackTest, PLA_Implied_zero)
    {
        // --- given
        cpu->A(0x3A);
        cpu->S(0xF6);
        testMemory[0x2000] = 0x68; // PLA

        testMemory[0x01F7] = 0x00;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0x01F7, cpu->S());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(0x00, testMemory[0x01F7]);
        EXPECT_EQ(InstructionTarget::STACK, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionStackTest, PLA_Implied_negative)
    {
        // --- given
        cpu->A(0x3A);
        cpu->S(0xF6);
        testMemory[0x2000] = 0x68; // PLA

        testMemory[0x01F7] = 0xCE;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0xCE, cpu->A());
        EXPECT_EQ(0x01F7, cpu->S());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0xCE, testMemory[0x01F7]);
        EXPECT_EQ(InstructionTarget::STACK, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionStackTest, PHP_Implied)
    {
        // --- given
        cpu->P(0b00100000);
        cpu->S(0xF2);
        testMemory[0x2000] = 0x08; // PHP (no flags)
        // auto oldCpu = CPU(cpu);
        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x01F1, cpu->S());

        //
        // EXPECT_EQ(0b00110000, cpu->P());
        // EXPECT_EQ(0b00110000, testMemory[0x01F2]); // PHP forces B to be 1
        EXPECT_EQ(0b00100000, testMemory[0x01F2]); // PHP forces B to be 1
        EXPECT_EQ(InstructionTarget::PSR, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::STACK, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionStackTest, PLP_Implied)
    {
        // --- given
        cpu->P(0xFF);
        cpu->S(0xF1);
        testMemory[0x2000] = 0x28; // PLP

        testMemory[0x01F2] = 0x33;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x33, cpu->P());
        EXPECT_EQ(0b00110011, cpu->P());
        EXPECT_EQ(0x01F2, cpu->S());
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
