

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionTransferTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("TransferUnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionTransferTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionTransferTest() override
        {
            delete cpu;
        }
    };

    // ========== Instructions ==========

    // ----- Transfer (load) -----
    // ----- Transfer (store) -----
    // ----- Transfer (interregister transfer) -----

    //----------------------------------------
    // Register Instructions
    //
    // Affect Flags: N Z
    //
    // These instructions are implied mode, have a length of one byte and require two machine cycles.
    //
    // MNEMONIC                 HEX
    // TAX (Transfer A to X)    $AA
    // TXA (Transfer X to A)    $8A
    // DEX (DEcrement X)        $CA     ; see incrementTest.cpp
    // INX (INcrement X)        $E8     ; see incrementTest.cpp
    // TAY (Transfer A to Y)    $A8
    // TYA (Transfer Y to A)    $98
    // DEY (DEcrement Y)        $88     ; see incrementTest.cpp
    // INY (INcrement Y)        $C8     ; see incrementTest.cpp
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ----- Implied
    TEST_F(InstructionTransferTest, TAX_Implied)
    {
        // --- given
        cpu->A(0x99);
        cpu->X(0x10);
        cpu->Y(0x10);
        testMemory.write(0x2000, 0xAA); // TAX

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x99, cpu->X());
        EXPECT_EQ(0x99, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionTransferTest, TXA_Implied)
    {
        // --- given
        cpu->A(0x99);
        cpu->X(0x10);
        cpu->Y(0x10);
        testMemory.write(0x2000, 0x8A); // TXA

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x10, cpu->X());
        EXPECT_EQ(0x10, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionTransferTest, TAY_Implied)
    {
        // --- given
        cpu->A(0x99);
        cpu->X(0x10);
        cpu->Y(0x10);
        testMemory.write(0x2000, 0xA8); // TAY

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x99, cpu->Y());
        EXPECT_EQ(0x99, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionTransferTest, TYA_Implied)
    {
        // --- given
        cpu->A(0x99);
        cpu->X(0x10);
        cpu->Y(0x30);
        testMemory.write(0x2000, 0x98); // TYA

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x30, cpu->Y());
        EXPECT_EQ(0x30, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
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
