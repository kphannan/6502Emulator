

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionShiftTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionShiftTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionShiftTest() override
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
    // ----- Shift & Rotate () -----

    //----------------------------------------
    // ASL (Arithmetic Shift Left)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   ASL A         $0A  1   2
    // Zero Page     ASL $44       $06  2   5
    // Zero Page,X   ASL $44,X     $16  2   6
    // Absolute      ASL $4400     $0E  3   6
    // Absolute,X    ASL $4400,X   $1E  3   7
    //
    // ASL shifts all bits left one position. 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionShiftTest, ASL_Immediate)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionShiftTest, ASL_ZeroPage)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionShiftTest, ASL_ZeroPageX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionShiftTest, ASL_Absolute)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionShiftTest, ASL_AbsoluteX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionShiftTest, ASL_AbsoluteY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionShiftTest, ASL_IndirectX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionShiftTest, ASL_IndirectY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    //----------------------------------------
    // LSR (Logical Shift Right)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   LSR A         $4A  1   2
    // Zero Page     LSR $44       $46  2   5
    // Zero Page,X   LSR $44,X     $56  2   6
    // Absolute      LSR $4400     $4E  3   6
    // Absolute,X    LSR $4400,X   $5E  3   7
    //
    // LSR shifts all bits right one position. 0 is shifted into bit 7 and the original bit 0 is shifted into the Carry.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionShiftTest, LSR_Immediate)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionShiftTest, LSR_ZeroPage)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionShiftTest, LSR_ZeroPageX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionShiftTest, LSR_Absolute)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionShiftTest, LSR_AbsoluteX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionShiftTest, LSR_AbsoluteY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionShiftTest, LSR_IndirectX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionShiftTest, LSR_IndirectY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    //----------------------------------------
    // ROL (ROtate Left)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   ROL A         $2A  1   2
    // Zero Page     ROL $44       $26  2   5
    // Zero Page,X   ROL $44,X     $36  2   6
    // Absolute      ROL $4400     $2E  3   6
    // Absolute,X    ROL $4400,X   $3E  3   7
    //
    // ROL shifts all bits left one position. The Carry is shifted into bit 0 and the original bit 7 is shifted into the Carry.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionShiftTest, ROL_Immediate)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionShiftTest, ROL_ZeroPage)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionShiftTest, ROL_ZeroPageX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionShiftTest, ROL_Absolute)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionShiftTest, ROL_AbsoluteX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionShiftTest, ROL_AbsoluteY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionShiftTest, ROL_IndirectX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionShiftTest, ROL_IndirectY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    //----------------------------------------
    // ROR (ROtate Right)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   ROR A         $6A  1   2
    // Zero Page     ROR $44       $66  2   5
    // Zero Page,X   ROR $44,X     $76  2   6
    // Absolute      ROR $4400     $6E  3   6
    // Absolute,X    ROR $4400,X   $7E  3   7
    //
    // ROR shifts all bits right one position. The Carry is shifted into bit 7 and the original bit 0 is shifted into the Carry.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionShiftTest, ROR_Immediate)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionShiftTest, ROR_ZeroPage)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionShiftTest, ROR_ZeroPageX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionShiftTest, ROR_Absolute)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionShiftTest, ROR_AbsoluteX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionShiftTest, ROR_AbsoluteY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionShiftTest, ROR_IndirectX)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionShiftTest, ROR_IndirectY)
    {
        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ----- Flag () -----
    // ----- Comparison () -----
    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
