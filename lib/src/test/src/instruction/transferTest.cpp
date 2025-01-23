

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionTransferTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
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
    // DEX (DEcrement X)        $CA
    // INX (INcrement X)        $E8
    // TAY (Transfer A to Y)    $A8
    // TYA (Transfer Y to A)    $98
    // DEY (DEcrement Y)        $88
    // INY (INcrement Y)        $C8
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ----- Implied
    TEST_F(InstructionTransferTest, TAX_Implied)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    TEST_F(InstructionTransferTest, TXA_Implied)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    TEST_F(InstructionTransferTest, DEX_Implied)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    TEST_F(InstructionTransferTest, INX_Implied)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    TEST_F(InstructionTransferTest, TAY_Implied)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    TEST_F(InstructionTransferTest, TYA_Implied)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    TEST_F(InstructionTransferTest, DEY_Implied)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    TEST_F(InstructionTransferTest, INY_Implied)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
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
