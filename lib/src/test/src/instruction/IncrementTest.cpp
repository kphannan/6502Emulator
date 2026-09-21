

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
    class InstructionIncrementTest : public testing::Test
    {
    public:
        memory::Memory testMemory = *(new memory::Memory( "UnitTest - Increment" ));

        CPU *cpu;

    protected:
        InstructionIncrementTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory[0x2000] = 0x49; // LDA #00 // starting instruction after reset
            testMemory[0x2001] = 0x5A;

            // Reset vector points to start of memory
            testMemory[0xFFFC] = 0x00; // cpu::HardwareVector::RESET
            testMemory[0xFFFD] = 0x20; //      MSB

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
        // --- given
        testMemory[0x2000] = 0xC6; // DEC $72
        testMemory[0x2001] = 0x72;

        testMemory[0x0072] = 0x74; //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x73, testMemory.read(0x0072));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, DEC_ZeroPageZero)
    {
        // --- given
        testMemory[0x2000] = 0xC6; // DEC $72
        testMemory[0x2001] = 0x72;

        testMemory[0x0072] = 0x01; //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(0x00, testMemory.read(0x0072));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, DEC_ZeroPageNegative)
    {
        // --- given
        testMemory[0x2000] = 0xC6; // DEC $72
        testMemory[0x2001] = 0x72;

        testMemory[0x0072] = 0x00; //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0xFF, testMemory.read(0x0072));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionIncrementTest, DEC_ZeroPageX)
    {
        // --- given
        cpu->X(0x10);
        testMemory[0x2000] = 0xD6; // DEC $72,X
        testMemory[0x2001] = 0x72;

        testMemory[0x0082] = 0x78; //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x77, testMemory.read(0x0082));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionIncrementTest, DEC_Absolute)
    {
        // --- given
        testMemory[0x2000] = 0xCE; // DEC $2233
        testMemory[0x2001] = 0x33;
        testMemory[0x2002] = 0x22;

        testMemory[0x2233] = 0x54; //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x53, testMemory.read(0x2233));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionIncrementTest, DEC_AbsoluteX)
    {
        // --- given
        cpu->X(0x10);
        testMemory[0x2000] = 0xDE; // DEC $2233,X
        testMemory[0x2001] = 0x33;
        testMemory[0x2002] = 0x22;

        testMemory[0x2243] = 0x19; //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x18, testMemory.read(0x2243));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // Register Instructions
    //
    // Affect Flags: N Z
    //
    // These instructions are implied mode, have a length of one byte and require two machine cycles.
    //
    // MNEMONIC                 HEX
    // DEX (DEcrement X)        $CA     ; see incrementTest.cpp
    // INX (INcrement X)        $E8     ; see incrementTest.cpp
    // DEY (DEcrement Y)        $88     ; see incrementTest.cpp
    // INY (INcrement Y)        $C8     ; see incrementTest.cpp
    //----------------------------------------

    TEST_F(InstructionIncrementTest, DEX_Implied)
    {
        // --- given
        cpu->X(0x10);
        testMemory[0x2000] = 0xCA; // DEX

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x0F, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, DEX_ImpliedZero)
    {
        // --- given
        cpu->X(0x01);
        testMemory[0x2000] = 0xCA; // DEX

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x00, cpu->X());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, DEX_ImpliedNegative)
    {
        // --- given
        cpu->X(0x00);
        testMemory[0x2000] = 0xCA; // DEX

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0xFF, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, DEY_Implied)
    {
        // --- given
        cpu->Y(0x20);
        testMemory[0x2000] = 0x88; // DEY

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x1F, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, DEY_ImpliedZero)
    {
        // --- given
        cpu->Y(0x01);
        testMemory[0x2000] = 0x88; // DEY

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x00, cpu->Y());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, DEY_ImpliedNegative)
    {
        // --- given
        cpu->Y(0x00);
        testMemory[0x2000] = 0x88; // DEY

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0xFF, cpu->Y());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

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
        // --- given
        testMemory[0x2000] = 0xE6; // INC $72
        testMemory[0x2001] = 0x72;

        testMemory[0x0072] = 0x74; //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x75, testMemory.read(0x0072));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, INC_ZeroPageZero) // TODO add tests for <,= 0
    {
        // --- given
        testMemory[0x2000] = 0xE6; // DEC $72
        testMemory[0x2001] = 0x72;

        testMemory[0x0072] = 0xFF; //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(0x00, testMemory.read(0x0072));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, INC_ZeroPageNegative) // TODO add tests for <,= 0
    {
        // --- given
        testMemory[0x2000] = 0xE6; // DEC $72
        testMemory[0x2001] = 0x72;

        testMemory[0x0072] = 0x7F; // Largest positive value

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0x80, testMemory.read(0x0072));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionIncrementTest, INC_ZeroPageX)
    {
        // --- given
        cpu->X(0x04);
        testMemory[0x2000] = 0xF6; // INC $72,X
        testMemory[0x2001] = 0x72;

        testMemory[0x0076] = 0x01; //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x02, testMemory.read(0x0076));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionIncrementTest, INC_Absolute)
    {
        // --- given
        cpu->X(0x04);
        testMemory[0x2000] = 0xEE; // INC $72,X
        testMemory[0x2001] = 0x44;
        testMemory[0x2002] = 0x72;

        testMemory[0x7244] = 0x63; //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x64, testMemory.read(0x7244));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionIncrementTest, INC_AbsoluteX)
    {
        // --- given
        cpu->X(0x04);
        testMemory[0x2000] = 0xFE; // INC $7231,X
        testMemory[0x2001] = 0x31;
        testMemory[0x2002] = 0x72;

        testMemory[0x7235] = 0x08; //

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x04, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x09, testMemory.read(0x7235));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // Register Instructions
    //
    // Affect Flags: N Z
    //
    // These instructions are implied mode, have a length of one byte and require two machine cycles.
    //
    // MNEMONIC                 HEX
    // INX (INcrement X)        $E8     ; see incrementTest.cpp
    // INY (INcrement Y)        $C8     ; see incrementTest.cpp
    //----------------------------------------

    TEST_F(InstructionIncrementTest, INX_Implied)
    {
        // --- given
        cpu->X(0x10);
        testMemory[0x2000] = 0xE8; // INX

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x11, cpu->X());
        EXPECT_EQ(0x00, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, INX_ImpliedZero)
    {
        // --- given
        cpu->X(0xFF);
        testMemory[0x2000] = 0xE8; // INX

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x00, cpu->X());
        EXPECT_EQ(0x00, cpu->Y());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, INX_ImpliedNegative)
    {
        // --- given
        cpu->X(0x7F);
        testMemory[0x2000] = 0xE8; // DEX

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x80, cpu->X());
        EXPECT_EQ(0x00, cpu->Y());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::X, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, INY_Implied)
    {
        // --- given
        cpu->Y(0x10);
        testMemory[0x2000] = 0xC8; // INY

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x00, cpu->X());
        EXPECT_EQ(0x11, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionIncrementTest, INY_ImpliedZero)
    {
        // --- given
        cpu->Y(0xFF);
        testMemory[0x2000] = 0xC8; // INY

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x00, cpu->X());
        EXPECT_EQ(0x00, cpu->Y());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
        // ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    TEST_F(InstructionIncrementTest, INY_ImpliedNegative)
    {
        // --- given
        cpu->Y(0x7F);
        testMemory[0x2000] = 0xC8; // INY

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x00, cpu->X());
        EXPECT_EQ(0x80, cpu->Y());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::Y, cpu->decodePipeline().dst);
        //        ADD_FAILURE_AT(__FILE__, __LINE__);
    }

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
