

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionIncrementTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionIncrementTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionIncrementTest() override
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

    //----------------------------------------
    // DEC (DECrement memory)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     DEC $44       $C6  2   5
    // Zero Page,X   DEC $44,X     $D6  2   6
    // Absolute      DEC $4400     $CE  3   6
    // Absolute,X    DEC $4400,X   $DE  3   7
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionIncrementTest, DEC_ZeroPage)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionIncrementTest, DEC_ZeroPageX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionIncrementTest, DEC_Absolute)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionIncrementTest, DEC_AbsoluteX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // INC (INCrement memory)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     INC $44       $E6  2   5
    // Zero Page,X   INC $44,X     $F6  2   6
    // Absolute      INC $4400     $EE  3   6
    // Absolute,X    INC $4400,X   $FE  3   7
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionIncrementTest, INC_ZeroPage)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionIncrementTest, INC_ZeroPageX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionIncrementTest, INC_Absolute)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionIncrementTest, INC_AbsoluteX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

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
