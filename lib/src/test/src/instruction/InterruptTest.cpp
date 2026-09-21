

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
    class InstructionInterruptTest : public testing::Test
    {
    public:
        memory::Memory testMemory = *(new memory::Memory( "UnitTest - Interrupt" ));

        CPU *cpu;

    protected:
        InstructionInterruptTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory[0x2000] = 0x49; // LDA #00 // starting instruction after reset
            testMemory[0x2001] = 0x5A;

            // testMemory.writeWord(0x01FA, 0x0102); // NMI
            // testMemory.writeWord(0x01FC, 0x5566); // RESET
            // testMemory.writeWord(0x01FE, 0x0506); // IRQ
            // testMemory[0x01f0] = 0x01;
            // testMemory[0x01f8] = 0x02;
            // //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
            // // 0x01F0: 00 00 00 00 00 00 00 00
            // // 0x01F8: 00 00 01 02 55 66 05 06
            // testMemory.showMemory(0x1f0, 0x000f, "Stack - Peppered");

            // Reset vector points to start of memory
            testMemory[0xFFFC] = 0x00; // cpu::HardwareVector::RESET
            testMemory[0xFFFD] = 0x20; //      MSB
            // testMemory.writeWord(0xFFFC, 0x2000); // cpu::HardwareVector::RESET

            testMemory.writeWord(0xFFFE, 0xDEAD); // cpu::HardwareVector::IRQ
            testMemory.writeWord(0xFFFA, 0xBEEF); // cpu::HardwareVector::NMI

            // testMemory.writeWord(0x0FFF0, 0xABCD);
            // testMemory.write(0x0FFF8, 0x04);
            //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
            // 0xFFF0: 00 00 00 00 00 00 00 00
            // 0xFFF8: 00 00 BE EF 20 00 AD DE
            // 0xFFFA: EF BE 00 20 AD DE
            // testMemory.showMemory(0xFFFA, 0x0006, "Vectors");

            cpu = new CPU(testMemory);
        }

        ~InstructionInterruptTest() override
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

    //----------------------------------------
    // BRK (BReaK)
    //
    // Affects Flags: B
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Implied       BRK           $00  1   7
    //
    // BRK causes a non-maskable interrupt and increments the program counter by one.
    // Therefore an RTI will go to the address of the BRK +2 so that BRK may be used to
    // replace a two-byte instruction for debugging and the subsequent RTI will be correct.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ----- Implied
    TEST_F(InstructionInterruptTest, BRK_Implied)
    {
        // --- given
        testMemory.writeWord(0xFFFA, 0xBEEF); // cpu::HardwareVector::NMI
        testMemory.writeWord(0xFFFC, 0xFADE); // cpu::HardwareVector::RESET
        testMemory.writeWord(0xFFFE, 0xDEAD); // cpu::HardwareVector::IRQ <--- load this

        testMemory[0x2000] = 0x00; // BRK
        testMemory[0x2001] = 0x42; // signature byte

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // Push current PC 2002 on stack
        //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        // 0x01F8: 00 00 01 02 55 66 02 20

        //         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        // 0x01F0: 00 00 00 00 00 00 00 00
        // 0x01F8: 00 00 01 02 55 66 02 20
        // testMemory.showMemory(0x1f0, 0x000f, "Stack - post execution"); // Stack

        // Stack ff: 02         PC low
        //       fe: 20         PC high
        //       fd: 0b00100000 status register
        //       fc: <- stack pointer

        // --- then
        EXPECT_EQ(0x01FC, cpu->S());  // PC and status pused
        EXPECT_EQ(0xDEAD, cpu->PC()); // PC loaded with IRQ vector

        EXPECT_EQ(0x2002, testMemory.readAddress(0x01FE)); // TODO PC was pushed to stack
        EXPECT_EQ(0b00100000, testMemory[0x01FD]);                           // status register pushed
        EXPECT_EQ(0b00110100, cpu->P());                                     // final status register
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

    //----------------------------------------
    // RTI (ReTurn from Interrupt)
    //
    // Affects Flags: all
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Implied       RTI           $40  1   6
    //
    // RTI retrieves the Processor Status Word (flags) and the Program Counter from the stack in that order (interrupts push the PC first and then the PSW).
    // Note that unlike RTS, the return address on the stack is the actual address rather than the address-1.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ----- Implied
    TEST_F(InstructionInterruptTest, RTI_Implied)
    {
        // ADD_FAILURE_AT(__FILE__, __LINE__);
        // --- given
        testMemory[0x2000] = 0x40; // RTI
        testMemory[0x2001] = 0x42; // signature byte
        testMemory[0x2002] = 0xA9; // LDA      <-  resume here

        // stack
        // Setup a stack frame of a BRK with P and PC on stack.
        // stack pointer is 3 bytes from the top
        cpu->S(0xFC);
        testMemory.writeWord(0x01FE, 0x2002); // Program counter
        testMemory.write(0x01FD, 0b00110101); // status flags

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0b00110101, cpu->P()); // final status register
        EXPECT_EQ(0x01FF, cpu->S());
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xA9, testMemory.read(cpu->PC()));
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

    // ----- Other () -----
}
