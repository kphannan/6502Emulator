

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
    class InstructionJumpTest : public testing::Test
    {
    public:
        memory::Memory testMemory = *(new memory::Memory( "UnitTest - Jump" ));

        CPU *cpu;

    protected:
        InstructionJumpTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory[0x2000] = 0xA9; // LDA #00 // starting instruction after reset
            testMemory[0x2001] = 0x00;

            // Reset vector points to start of memory
            testMemory[0xFFFC] = 0x00; // cpu::HardwareVector::RESET
            testMemory[0xFFFD] = 0x20; //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionJumpTest() override
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

    //----------------------------------------
    // JMP (JuMP)
    //
    // Affects Flags: none
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Absolute      JMP $5597     $4C  3   3
    // Indirect      JMP ($5597)   $6C  3   5
    //
    // JMP transfers program execution to the following address (absolute)
    // or to the location contained in the following address (indirect).
    // Note that there is no carry associated with the indirect jump so:
    // AN INDIRECT JUMP MUST NEVER USE A
    // VECTOR BEGINNING ON THE LAST BYTE
    // OF A PAGE
    // For example if address $3000 contains $40, $30FF contains $80,
    // and $3100 contains $50, the result of JMP ($30FF) will be a
    // transfer of control to $4080 rather than $5080 as you intended
    // i.e. the 6502 took the low byte of the address from $30FF and
    // the high byte from $3000.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator
    // ..... ZeroPage $LL
    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionJumpTest, JMP_Absolute)
    {
        // TODO add test for the CPU bug; unable to read absolute address across a page boundary
        // ADD_FAILURE_AT(__FILE__, __LINE__);
        // testMemory.writeWord(0x2001, 0x5A81);
        // --- given
        testMemory[0x2000] = 0x4C; // JMP $5597 // starting instruction after reset
        testMemory[0x2001] = 0x97; // low byte of target address
        testMemory[0x2002] = 0x55; // high byte of target address

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x5597, cpu->PC());
    }

    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y

    // ----- Indirect ($LLHH)
    TEST_F(InstructionJumpTest, JMP_Indirect)
    {
        // --- given
        testMemory[0x2000] = 0x6C; // JMP $5597 // starting instruction after reset
        testMemory[0x2001] = 0x10; // low byte of target address
        testMemory[0x2002] = 0xA0; // high byte of target address

        testMemory.writeWord(0xA010, 0x3366); // high byte of target address
        testMemory.writeWord(0x10A0, 0xAABB); // high byte of target address

        // --- when
        cpu->executeFromAddress(0x2000, 1);
        // --- then
        EXPECT_EQ(0x3366, cpu->PC());
    }

    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // JSR (Jump to SubRoutine)
    //
    // Affects Flags: none
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Absolute      JSR $5597     $20  3   6
    //
    // JSR pushes the address-1 of the next operation on to the stack before transferring program control to the following address. Subroutines are normally terminated by a RTS op code.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator
    // ..... ZeroPage $LL
    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionJumpTest, JSR_Absolute)
    {
        // --- given
        testMemory[0x2000] = 0x20; // JMP $5597 // starting instruction after reset
        testMemory[0x2001] = 0x10; // low byte of target address
        testMemory[0x2002] = 0xA0; // high byte of target address
        // 0x2003 --- execution resumes here ---

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        testMemory.showMemory(0x01f8, 0x08, "Stack after JSR absolute");
        EXPECT_EQ(0xA010, cpu->PC());
        // verify stack contents and stack pointer
        EXPECT_EQ(0x01FD, cpu->S());

        EXPECT_EQ(0x2002, (hardware::Address)(testMemory.readWord(0x01FE))); // TODO PC was pushed to stack
        EXPECT_EQ(0b00100000, cpu->P());                                     // final status register
    }

    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // RTS (ReTurn from Subroutine)
    //
    // Affects Flags: none
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Implied       RTS           $60  1   6
    //
    // RTS pulls the top two bytes off the stack (low byte first) and transfers program control to that address+1. It is used, as expected, to exit a subroutine invoked via JSR which pushed the address-1.
    // RTS is frequently used to implement a jump table where addresses-1 are pushed onto the stack and accessed via RTS eg. to access the second of four routines:
    //
    //  LDX #1
    //  JSR EXEC
    //  JMP SOMEWHERE
    //
    // LOBYTE
    //  .BYTE <ROUTINE0-1,<ROUTINE1-1
    //  .BYTE <ROUTINE2-1,<ROUTINE3-1
    //
    // HIBYTE
    //  .BYTE >ROUTINE0-1,>ROUTINE1-1
    //  .BYTE >ROUTINE2-1,>ROUTINE3-1
    //
    // EXEC
    //  LDA HIBYTE,X
    //  PHA
    //  LDA LOBYTE,X
    //  PHA
    //  RTS
    //
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB

    // ----- Implied
    TEST_F(InstructionJumpTest, RTS_Implied)
    {
        // --- given
        // Program          Stack
        // 0x2000 0x20      0x01FF:   0x02
        // 0x2001 0x10      0x01FE:   0x20
        // 0x2002 0xA0      0x01FD:    <-- Stack Pointer
        //    . . . .
        // 0xA010 <-- PC

        // testMemory[0x2000] = 0x20; // JMP $5597 // starting instruction after reset
        // testMemory[0x2001] = 0x10; // low byte of target address
        // testMemory[0x2002] = 0xA0; // high byte of target address
        // 0x2003 --- execution resumes here ---

        // TODO revisit how bytes are  pushed / popped from the stack.
        testMemory[0x01FF] = 0x20; // 0x2003 // starting instruction after return (-1)
        testMemory[0x01FE] = 0x02; //
        cpu->S(0xFD);

        testMemory[0xA010] = 0x60; // RTS -- starting instruction
                                   //        cpu->PC( 0xA010 );

        // --- when
        cpu->executeFromAddress(0xA010, 1);

        // --- then
        // Program          Stack
        // 0x2000 0x20      0x01FF:     <-- Stack Pointer
        // 0x2001 0x10      0x01FE:
        // 0x2002 0xA0      0x01FD:
        // 0x2003   <-- PC

        // 0xA010 0x60

        EXPECT_EQ(0x2003, cpu->PC());
        // verify stack contents and stack pointer
        EXPECT_EQ(0x01FF, cpu->S());

        EXPECT_EQ(0b00100000, cpu->P()); // final status register
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

    // ----- Interrupt () -----
    // ----- Other () -----
}
