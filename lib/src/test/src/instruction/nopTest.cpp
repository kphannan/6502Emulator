

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
    class InstructionNoOpTest : public testing::Test
    {
    public:
        memory::Memory testMemory = *(new memory::Memory( "UnitTest - No Op" ));

        CPU *cpu;

    protected:
        InstructionNoOpTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory[0x2000] = 0x49; // LDA #00 // starting instruction after reset
            testMemory[0x2001] = 0x5A;

            // Reset vector points to start of memory
            testMemory[0xFFFC] = 0x00; // cpu::HardwareVector::RESET
            testMemory[0xFFFD] = 0x20; //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionNoOpTest() override
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
    // ----- Flag () -----
    // ----- Comparison () -----
    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----

    //----------------------------------------
    // NOP (No OPeration)
    //
    // Affects Flags: none
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Implied       NOP           $EA  1   2
    //
    // NOP is used to reserve space for future modifications or effectively REM out existing code.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ----- Implied
    TEST_F(InstructionNoOpTest, NOP_Implied)
    {
        //        ADD_FAILURE_AT(__FILE__, __LINE__);
        // --- given
        testMemory[0x2000] = 0xEA; // NOP

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
    }

    /*
     // --- given
     cpu->A(0x22); // 0010 0010 -> 0100 0100 C:0
     cpu->setC();
     testMemory[0x2000] = 0x0A; // ASL A

     // --- when
     cpu->executeFromAddress(0x2000, 1);

     // --- then
     EXPECT_EQ(0x2001, cpu->PC());
     EXPECT_EQ(0x44, cpu->A());
     EXPECT_EQ(0b00100000, cpu->P());
     EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
     EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);

     */

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

}
