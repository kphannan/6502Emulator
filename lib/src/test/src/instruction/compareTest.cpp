

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionCompareTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionCompareTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionCompareTest() override
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

    //----------------------------------------
    // CMP (CoMPare accumulator)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     CMP #$44      $C9  2   2
    // Zero Page     CMP $44       $C5  2   3
    // Zero Page,X   CMP $44,X     $D5  2   4
    // Absolute      CMP $4400     $CD  3   4
    // Absolute,X    CMP $4400,X   $DD  3   4+
    // Absolute,Y    CMP $4400,Y   $D9  3   4+
    // Indirect,X    CMP ($44,X)   $C1  2   6
    // Indirect,Y    CMP ($44),Y   $D1  2   5+
    //
    // + add 1 cycle if page boundary crossed
    //
    // Compare sets flags as if a subtraction had been carried out. If the value in the accumulator is equal or greater than the compared value, the Carry will be set. The equal (Z) and negative (N) flags will be set based on equality or lack thereof and the sign (i.e. A>=$80) of the accumulator.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionCompareTest, CMP_Immediate)
    {
        // --- given
        cpu->A(0x74);
        testMemory.write(0x2000, 0xC9); // CMP #$23
        testMemory.write(0x2001, 0x23);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionCompareTest, CMP_ImmediateEqual)
    {
        // --- given
        cpu->A(0x23);
        testMemory.write(0x2000, 0xC9); // CMP #$23
        testMemory.write(0x2001, 0x23);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionCompareTest, CMP_ImmediateLessThan)
    {
        // --- given
        cpu->A(0x12);
        testMemory.write(0x2000, 0xC9); // CMP #$23
        testMemory.write(0x2001, 0x23);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b10100001, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionCompareTest, CMP_ZeroPage)
    {
        // --- given
        cpu->A(0xFF);
        cpu->X(0x20);
        testMemory.write(0x2000, 0xC5); // CMP $23
        testMemory.write(0x2001, 0x23);

        // Base
        testMemory.write(0x0023, 0x77); // 0xFF - 0x77 = 0x88 (Z:0 S:1 C:0)

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionCompareTest, CMP_ZeroPageX)
    {
        // --- given
        cpu->A(0xFF);
        cpu->X(0x20);
        testMemory.write(0x2000, 0xD5); // CMP ($23,X)
        testMemory.write(0x2001, 0x23);

        // Base
        testMemory.write(0x0023, 0x77); // table base address

        testMemory.write(0x0043, 0x6D); // X + $23
        testMemory.write(0x0044, 0x15);

        testMemory.write(0x156D, 0x18); // 0xFF - 0x18 = 0xE7  (Z:0, S:1, C:1)

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionCompareTest, CMP_Absolute)
    {
        // --- given
        cpu->A(0xFF);
        testMemory.write(0x2000, 0xDD); // CMP $E070
        testMemory.write(0x2001, 0x70);
        testMemory.write(0x2002, 0xE0);

        // Base
        testMemory.write(0xE070, 0x77); // table base address

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionCompareTest, CMP_AbsoluteX)
    {
        // --- given
        cpu->A(0xFF);
        cpu->X(0x20);
        testMemory.write(0x2000, 0xDD); // CMP $2354,X
        testMemory.write(0x2001, 0x54);
        testMemory.write(0x2002, 0x23);

        // Base
        testMemory.write(0x2354, 0x77); // table base address
        testMemory.write(0x2374, 0x77); // table base address

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionCompareTest, CMP_AbsoluteY)
    {
        // --- given
        cpu->A(0xFF);
        cpu->Y(0x08);
        testMemory.write(0x2000, 0xD9); // CMP $8020,Y
        testMemory.write(0x2001, 0x20);
        testMemory.write(0x2002, 0x80);

        // Base
        testMemory.write(0x8028, 0x66); // table base address

        // testMemory.write(0x0043, 0x6D); // X + $23
        // testMemory.write(0x0044, 0x15);

        // testMemory.write(0x156D, 0x18); // 0xFF - 0x18 = 0xE7  (Z:0, S:1, C:1)

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionCompareTest, CMP_IndirectX)
    {
        // --- given
        cpu->A(0xFF);
        cpu->X(0x20);
        testMemory.write(0x2000, 0xC1); // CMP ($54,X)
        testMemory.write(0x2001, 0x54);
        // testMemory.write(0x2002, 0x23);

        // Base
        testMemory.write(0x2354, 0x11); // table base address
        // ... . ..... .....
        testMemory.write(0x2374, 0x77); // $20th entry

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionCompareTest, CMP_IndirectY)
    {
        // --- given
        cpu->A(0xF0);
        cpu->Y(0x20);
        testMemory.write(0x2000, 0xC1); // CMP ($54,X)
        testMemory.write(0x2001, 0x54);
        // testMemory.write(0x2002, 0x23);

        // Indirection
        testMemory.write(0x0054, 0x11);
        testMemory.write(0x0055, 0x11);

        // Base
        testMemory.write(0x1111, 0x11); // table base address
        // ... . ..... .....
        testMemory.write(0x1130, 0x77); // $20th entry
        testMemory.write(0x1131, 0x33); // $20th entry

        // Data
        testMemory.write(0x3377, 0x77);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    //----------------------------------------
    // CPX (ComPare X register)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     CPX #$44      $E0  2   2
    // Zero Page     CPX $44       $E4  2   3
    // Absolute      CPX $4400     $EC  3   4
    //
    // Operation and flag results are identical to equivalent mode accumulator CMP ops.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionCompareTest, CPX_Immediate)
    {
        // --- given
        cpu->X(0x74);
        testMemory.write(0x2000, 0xE0); // CPX #$23
        testMemory.write(0x2001, 0x23);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionCompareTest, CPX_ZeroPage)
    {
        // --- given
        cpu->X(0xFF);
        testMemory.write(0x2000, 0xE4); // CPX $23
        testMemory.write(0x2001, 0x23);

        // Base
        testMemory.write(0x0023, 0x77); // 0xFF - 0x77 = 0x88 (Z:0 S:1 C:0)

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionCompareTest, CPX_Absolute)
    {
        // --- given
        cpu->X(0xFF);
        testMemory.write(0x2000, 0xEC); // CPX $E070
        testMemory.write(0x2001, 0x70);
        testMemory.write(0x2002, 0xE0);

        testMemory.write(0xE070, 0x77); // Data

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // CPY (ComPare Y register)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     CPY #$44      $C0  2   2
    // Zero Page     CPY $44       $C4  2   3
    // Absolute      CPY $4400     $CC  3   4
    //
    // Operation and flag results are identical to equivalent mode accumulator CMP ops.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionCompareTest, CPY_Immediate)
    {
        // --- given
        cpu->Y(0x74);
        testMemory.write(0x2000, 0xC0); // CPY #$23
        testMemory.write(0x2001, 0x23);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionCompareTest, CPY_ZeroPage)
    {
        // --- given
        cpu->Y(0xFF);
        testMemory.write(0x2000, 0xC4); // CPY $23
        testMemory.write(0x2001, 0x23);

        // Base
        testMemory.write(0x0023, 0x77); // 0xFF - 0x77 = 0x88 (Z:0 S:1 C:0)

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionCompareTest, CPY_Absolute)
    {
        // --- given
        cpu->Y(0xFF);
        testMemory.write(0x2000, 0xCC); // CPY $E070
        testMemory.write(0x2001, 0x70);
        testMemory.write(0x2002, 0xE0);

        testMemory.write(0xE070, 0x77); // Data

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
