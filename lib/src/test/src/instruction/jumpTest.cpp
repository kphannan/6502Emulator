

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionJumpTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionJumpTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0xA9); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x00);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

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
        testMemory.write(0x2000, 0x4C); // JMP $5597 // starting instruction after reset
        testMemory.write(0x2001, 0x97); // low byte of target address
        testMemory.write(0x2002, 0x55); // high byte of target address

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
        testMemory.write(0x2000, 0x6C); // JMP $5597 // starting instruction after reset
        testMemory.write(0x2001, 0x10); // low byte of target address
        testMemory.write(0x2002, 0xA0); // high byte of target address

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
        testMemory.write(0x2000, 0x20); // JMP $5597 // starting instruction after reset
        testMemory.write(0x2001, 0x10); // low byte of target address
        testMemory.write(0x2002, 0xA0); // high byte of target address
        // 0x2003 --- execution resumes here ---

        // --- when
        cpu->executeFromAddress(0x2000, 1);


        // --- then
        EXPECT_EQ(0xA010, cpu->PC());
        // verify stack contents and stack pointer
        EXPECT_EQ(0x01FD, cpu->S());

        EXPECT_EQ(0x2003, (hardware::Address)(testMemory.readWord(0x01FE))); // TODO PC was pushed to stack
        EXPECT_EQ(0b00100000, cpu->P());                                     // final status register
    }

/*
 // --- given
 testMemory.write(0x2000, 0x00); // BRK
 testMemory.write(0x2001, 0x42); // signature byte
 // --- when
 cpu->executeFromAddress(0x2000, 1);

 // Push current PC 2002 on stack
 //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
 // 0x01F8: 00 00 01 02 55 66 02 20

 //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
 // 0x01F0: 00 00 00 00 00 00 00 00
 // 0x01F8: 00 00 01 02 55 66 02 20
 testMemory.showMemory(0x1f0, 0x000f, "Stack - post execution"); // Stack

 // Stack ff: 02         PC low
 //       fe: 20         PC high
 //       fd: 0b00100000 status register
 //       fc: <- stack pointer

 // --- then
 EXPECT_EQ(0x01FC, cpu->S());  // PC and status pused
 EXPECT_EQ(0xDEAD, cpu->PC()); // PC loaded with IRQ vector

 EXPECT_EQ(0x2002, (hardware::Address)(testMemory.readWord(0x01FE))); // TODO PC was pushed to stack
 EXPECT_EQ(0b00100000, testMemory.read(0x01FD));                      // status register pushed
 EXPECT_EQ(0b00110100, cpu->P());                                     // final status register

 */



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
        // ADD_FAILURE_AT(__FILE__, __LINE__);
        // --- given
        // Program          Stack
        // 0x2000 0x20      0x01FF:   0x03
        // 0x2001 0x10      0x01FE:   0x20
        // 0x2002 0xA0      0x01FD:    <-- Stack Pointer
        //    . . . .
        // 0xA010 <-- PC

        testMemory.write(0x2000, 0x20); // JMP $5597 // starting instruction after reset
        testMemory.write(0x2001, 0x10); // low byte of target address
        testMemory.write(0x2002, 0xA0); // high byte of target address
        // 0x2003 --- execution resumes here ---

        // TODO revisit how bytes are  pushed / popped from the stack.
        testMemory.write(0x01FF, 0x20); // 0x2003 // starting instruction after reset
        testMemory.write(0x01FE, 0x03); //
        cpu->S( 0x01FD );

        testMemory.write(0xA010, 0x60); // RTS -- starting instruction 
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

        EXPECT_EQ(0b00100000, cpu->P());                                     // final status register
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
