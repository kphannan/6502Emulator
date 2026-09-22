

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
    class InstructionBranchTest : public testing::Test
    {
        public:
            memory::Memory testMemory = *(new memory::Memory( "UnitTest - Branching" ));

            CPU *cpu;

        protected:
            InstructionBranchTest()
            {
                // Destination of the reset vector - leaves zeroPage available for testing
                testMemory[0x2000] = 0x49; // LDA #00 // starting instruction after reset
                testMemory[0x2001] = 0x5A;

                // Reset vector points to start of memory
                testMemory.writeWord(0xFFFC, 0x2000); // cpu::HardwareVector::RESET
                testMemory.writeWord(0xFFFE, 0xDEAD); // cpu::HardwareVector::IRQ
                testMemory.writeWord(0xFFFA, 0xBEEF); // cpu::HardwareVector::NMI

                cpu = new CPU(testMemory);
            }

            ~InstructionBranchTest() override
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

    //----------------------------------------
    // Branch Instructions
    //
    // Affect Flags: none
    //
    // All branches are relative mode and have a length of two bytes. Syntax is "Bxx Displacement"
    // or (better) "Bxx Label". See the notes on the Program Counter for more on displacements.
    //
    // Branches are dependant on the status of the flag bits when the op code is encountered.
    // A branch not taken requires two machine cycles. Add one if the branch is taken and add
    // one more if the branch crosses a page boundary.
    //
    // MNEMONIC                       HEX
    // BPL (Branch on PLus)           $10
    // BMI (Branch on MInus)          $30
    // BVC (Branch on oVerflow Clear) $50
    // BVS (Branch on oVerflow Set)   $70
    // BCC (Branch on Carry Clear)    $90
    // BCS (Branch on Carry Set)      $B0
    // BNE (Branch on Not Equal)      $D0
    // BEQ (Branch on EQual)          $F0
    //
    // There is no BRA (BRanch Always) instruction but it can be easily emulated by branching
    // on the basis of a known condition. One of the best flags to use for this purpose is the
    //  oVerflow which is unchanged by all but addition and subtraction operations.
    // A page boundary crossing occurs when the branch destination is on a different page than
    // the instruction AFTER the branch instruction. For example:
    //
    //   SEC
    //   BCS LABEL
    //   NOP
    // A page boundary crossing occurs (i.e. the BCS takes 4 cycles) when (the address of)
    // LABEL and the NOP are on different pages. This means that
    //         CLV
    //         BVC LABEL
    //   LABEL NOP
    // the BVC instruction will take 3 cycles no matter what address it is located at.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator
    // ..... ZeroPage $LL
    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ----- Relative $BB

    // --- BPL ---
    TEST_F(InstructionBranchTest, BPL_Relative_No)
    {
        // --- given
        // cpu->clearN();
        cpu->setN();
        testMemory[0x2000] = 0x10; // BPL
        testMemory[0x2001] = 0x7F; // maximum forward
        // testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_N, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BPL_Relative_Forward_Max)
    {
        // --- given
        cpu->clearN();
        testMemory[0x2000] = 0x10; // BPL
        testMemory[0x2001] = 0x7F; // maximum forward

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2081, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_N, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BPL_Relative_Reverse_Max)
    {
        // --- given
        cpu->clearN();
        testMemory[0x2000] = 0x10; // BPL
        testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x1F82, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_N, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    // --- BMI ---
    TEST_F(InstructionBranchTest, BMI_Relative_No)
    {
        // --- given
        cpu->clearN();
        // cpu->setN();
        testMemory[0x2000] = 0x30; // BMI
        testMemory[0x2001] = 0x7F; // maximum forward
        // testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_N, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BMI_Relative_Forward_Max)
    {
        // --- given
        cpu->setN();
        testMemory[0x2000] = 0x30; // BMI
        testMemory[0x2001] = 0x7F; // maximum forward

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2081, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_N, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BMI_Relative_Reverse_Max)
    {
        // --- given
        cpu->setN();
        testMemory[0x2000] = 0x30; // BMI
        testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x1F82, cpu->PC());
        EXPECT_EQ(0b10100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_N, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    // --- BVC ---
    TEST_F(InstructionBranchTest, BVC_Relative_No)
    {
        // --- given
        cpu->setV();
        // cpu->setN();
        testMemory[0x2000] = 0x50; // BVC
        testMemory[0x2001] = 0x7F; // maximum forward
        // testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b01100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_V, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BVC_Relative_Forward_Max)
    {
        // --- given
        cpu->clearV();
        testMemory[0x2000] = 0x50; // BVC
        testMemory[0x2001] = 0x7F; // maximum forward

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2081, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_V, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BVC_Relative_Reverse_Max)
    {
        // --- given
        cpu->clearV();
        testMemory[0x2000] = 0x50; // BVC
        testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x1F82, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_V, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }


    // --- BVS ---
    TEST_F(InstructionBranchTest, BVS_Relative_No)
    {
        // --- given
        cpu->clearV();
        // cpu->setN();
        testMemory[0x2000] = 0x70; // BVS
        testMemory[0x2001] = 0x7F; // maximum forward
        // testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_V, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BVS_Relative_Forward_Max)
    {
        // --- given
        cpu->setV();
        testMemory[0x2000] = 0x70; // BVS
        testMemory[0x2001] = 0x7F; // maximum forward

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2081, cpu->PC());
        EXPECT_EQ(0b01100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_V, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BVS_Relative_Reverse_Max)
    {
        // --- given
        cpu->setV();
        testMemory[0x2000] = 0x70; // BVS
        testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x1F82, cpu->PC());
        EXPECT_EQ(0b01100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_V, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }





    // --- BCC ---
    TEST_F(InstructionBranchTest, BCC_Relative_No)
    {
        // --- given
        // cpu->clearC();
        cpu->setC();
        testMemory[0x2000] = 0x90; // BCC
        testMemory[0x2001] = 0x7F; // maximum forward
        // testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100001, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_C, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BCC_Relative_Forward_Max)
    {
        // --- given
        cpu->clearC();
        // cpu->setC();
        testMemory[0x2000] = 0x90; // BCC
        testMemory[0x2001] = 0x7F; // maximum forward
        // testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2081, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_C, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BCC_Relative_Reverse_Max)
    {
        // --- given
        cpu->clearC();
        // cpu->setC();
        testMemory[0x2000] = 0x90; // BCC
        // testMemory[0x2001] = 0x7F; // maximum forward
        testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x1F82, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_C, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    // --- BCS ---
    TEST_F(InstructionBranchTest, BCS_Relative_No)
    {
        // --- given
        cpu->clearC();
        // cpu->setC();
        testMemory[0x2000] = 0xB0; // BCS
        testMemory[0x2001] = 0x7F; // maximum forward
        // testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_C, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BCS_Relative_Forward_Max)
    {
        // --- given
        // cpu->clearC();
        cpu->setC();
        testMemory[0x2000] = 0xB0; // BCS
        testMemory[0x2001] = 0x7F; // maximum forward
        // testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2081, cpu->PC());
        EXPECT_EQ(0b00100001, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_C, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BCS_Relative_Reverse_Max)
    {
        // --- given
        // cpu->clearC();
        cpu->setC();
        testMemory[0x2000] = 0xB0; // BCS
        // testMemory[0x2001] = 0x7F; // maximum forward
        testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x1F82, cpu->PC());
        EXPECT_EQ(0b00100001, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_C, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }




    // --- BNE ---
    TEST_F(InstructionBranchTest, BNE_Relative_No)
    {
        // --- given
        cpu->setZ();
        // cpu->setN();
        testMemory[0x2000] = 0xD0; // BNE
        testMemory[0x2001] = 0x7F; // maximum forward
        // testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100010, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_Z, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BNE_Relative_Forward_Max)
    {
        // --- given
        cpu->clearZ();
        testMemory[0x2000] = 0xD0; // BNE
        testMemory[0x2001] = 0x7F; // maximum forward

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2081, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_Z, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BNE_Relative_Reverse_Max)
    {
        // --- given
        cpu->clearZ();
        testMemory[0x2000] = 0xD0; // BNE
        testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x1F82, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_Z, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }


    // --- BEQ ---
    TEST_F(InstructionBranchTest, BEQ_Relative_No)
    {
        // --- given
        cpu->clearZ();
        // cpu->setN();
        testMemory[0x2000] = 0xF0; // BEQ
        testMemory[0x2001] = 0x7F; // maximum forward
        // testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_Z, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BEQ_Relative_Forward_Max)
    {
        // --- given
        cpu->setZ();
        testMemory[0x2000] = 0xF0; // BEQ
        testMemory[0x2001] = 0x7F; // maximum forward

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2081, cpu->PC());
        EXPECT_EQ(0b00100010, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_Z, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionBranchTest, BEQ_Relative_Reverse_Max)
    {
        // --- given
        cpu->setZ();
        testMemory[0x2000] = 0xF0; // BEQ
        testMemory[0x2001] = 0x80; // maximum reverse

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x1F82, cpu->PC());
        EXPECT_EQ(0b00100010, cpu->P());

        EXPECT_EQ(InstructionTarget::FLAG_Z, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::PC, cpu->decodePipeline().dst);
    }








    // ..... Absolute $LLHH
    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
