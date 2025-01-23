

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionArithmeticTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionArithmeticTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionArithmeticTest() override
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

    //----------------------------------------
    // ADC (ADd with Carry)
    //
    // Affects Flags: N V Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     ADC #$44      $69  2   2
    // Zero Page     ADC $44       $65  2   3
    // Zero Page,X   ADC $44,X     $75  2   4
    // Absolute      ADC $4400     $6D  3   4
    // Absolute,X    ADC $4400,X   $7D  3   4+
    // Absolute,Y    ADC $4400,Y   $79  3   4+
    // Indirect,X    ADC ($44,X)   $61  2   6
    // Indirect,Y    ADC ($44),Y   $71  2   5+
    //
    // + add 1 cycle if page boundary crossed
    //
    // ADC results are dependant on the setting of the decimal flag. In decimal mode, addition is carried out on the assumption that the values involved are packed BCD (Binary Coded Decimal).
    // There is no way to add without carry.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionArithmeticTest, ADC_Immediate)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionArithmeticTest, ADC_ZeroPage)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionArithmeticTest, ADC_ZeroPageX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionArithmeticTest, ADC_Absolute)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionArithmeticTest, ADC_AbsoluteX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionArithmeticTest, ADC_AbsoluteY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionArithmeticTest, ADC_IndirectX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionArithmeticTest, ADC_IndirectY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    //----------------------------------------
    // SBC (SuBtract with Carry)
    //
    // Affects Flags: N V Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     SBC #$44      $E9  2   2
    // Zero Page     SBC $44       $E5  2   3
    // Zero Page,X   SBC $44,X     $F5  2   4
    // Absolute      SBC $4400     $ED  3   4
    // Absolute,X    SBC $4400,X   $FD  3   4+
    // Absolute,Y    SBC $4400,Y   $F9  3   4+
    // Indirect,X    SBC ($44,X)   $E1  2   6
    // Indirect,Y    SBC ($44),Y   $F1  2   5+
    //
    // + add 1 cycle if page boundary crossed
    //
    // SBC results are dependant on the setting of the decimal flag. In decimal mode, subtraction is carried out on the assumption that the values involved are packed BCD (Binary Coded Decimal).
    // There is no way to subtract without the carry which works as an inverse borrow. i.e, to subtract you set the carry before the operation. If the carry is cleared by the operation, it indicates a borrow occurred.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionArithmeticTest, SBC_Immediate)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionArithmeticTest, SBC_ZeroPage)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionArithmeticTest, SBC_ZeroPageX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionArithmeticTest, SBC_Absolute)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionArithmeticTest, SBC_AbsoluteX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionArithmeticTest, SBC_AbsoluteY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionArithmeticTest, SBC_IndirectX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionArithmeticTest, SBC_IndirectY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- Logical () -----
    // ----- Shift & Rotate () -----
    // ----- Flag () -----
    // ----- Comparison () -----

    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
