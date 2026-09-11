

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
    class InstructionFlagTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionFlagTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionFlagTest() override
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

    //----------------------------------------
    // Flag (Processor Status) Instructions
    //
    // Affect Flags: as noted
    //
    // These instructions are implied mode, have a length of one byte and require two machine cycles.
    //
    // MNEMONIC                       HEX
    // CLC (CLear Carry)              $18
    // SEC (SEt Carry)                $38
    // CLI (CLear Interrupt)          $58
    // SEI (SEt Interrupt)            $78
    // CLV (CLear oVerflow)           $B8
    // CLD (CLear Decimal)            $D8
    // SED (SEt Decimal)              $F8
    //
    // Notes:
    //   The Interrupt flag is used to prevent (SEI) or enable (CLI) maskable interrupts (aka IRQ's). It does not signal the presence or absence of an interrupt condition. The 6502 will set this flag automatically in response to an interrupt and restore it to its prior status on completion of the interrupt service routine. If you want your interrupt service routine to permit other maskable interrupts, you must clear the I flag in your code.
    //
    //   The Decimal flag controls how the 6502 adds and subtracts. If set, arithmetic is carried out in packed binary coded decimal. This flag is unchanged by interrupts and is unknown on power-up. The implication is that a CLD should be included in boot or interrupt coding.
    //
    //   The Overflow flag is generally misunderstood and therefore under-utilised. After an ADC or SBC instruction, the overflow flag will be set if the twos complement result is less than -128 or greater than +127, and it will cleared otherwise. In twos complement, $80 through $FF represents -128 through -1, and $00 through $7F represents 0 through +127. Thus, after:
    //
    //   CLC
    //   LDA #$7F ;   +127
    //   ADC #$01 ; +   +1
    // the overflow flag is 1 (+127 + +1 = +128), and after:
    //   CLC
    //   LDA #$81 ;   -127
    //   ADC #$FF ; +   -1
    // the overflow flag is 0 (-127 + -1 = -128). The overflow flag is not affected by increments, decrements, shifts and logical operations i.e. only ADC, BIT, CLV, PLP, RTI and SBC affect it. There is no op code to set the overflow but a BIT test on an RTS instruction will do the trick.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ----- Implied
    TEST_F(InstructionFlagTest, CLC_Implied)
    {
        // --- given
        cpu->setC();
        testMemory.write(0x2000, 0x18); // CLC

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_FALSE(cpu->isC());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::FLAG_C, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::FLAG_C, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionFlagTest, SEC_Implied)
    {
        // --- given
        cpu->clearC();
        testMemory.write(0x2000, 0x38); // SEC

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_TRUE(cpu->isC());
        EXPECT_EQ(0b00100001, cpu->P());
        EXPECT_EQ(InstructionTarget::FLAG_C, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::FLAG_C, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionFlagTest, CLI_Implied)
    {
        // --- given
        cpu->setI();
        testMemory.write(0x2000, 0x58); // CLI

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_FALSE(cpu->isI());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::FLAG_I, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::FLAG_I, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionFlagTest, SEI_Implied)
    {
        // --- given
        cpu->clearI();
        testMemory.write(0x2000, 0x78); // SEI

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_TRUE(cpu->isI());
        EXPECT_EQ(0b00100100, cpu->P());
        EXPECT_EQ(InstructionTarget::FLAG_I, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::FLAG_I, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionFlagTest, CLV_Implied)
    {
        // --- given
        cpu->setV();
        testMemory.write(0x2000, 0xB8); // CLV

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_FALSE(cpu->isV());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::FLAG_V, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::FLAG_V, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionFlagTest, CLD_Implied)
    {
        // --- given
        cpu->setD();
        testMemory.write(0x2000, 0xD8); // CLD

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_FALSE(cpu->isD());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::FLAG_D, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::FLAG_D, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionFlagTest, SED_Implied)
    {
        // --- given
        cpu->setD();
        testMemory.write(0x2000, 0xF8); // SED

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_TRUE(cpu->isD());
        EXPECT_EQ(0b00101000, cpu->P());
        EXPECT_EQ(InstructionTarget::FLAG_D, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::FLAG_D, cpu->decodePipeline().dst);
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

    // ----- Comparison () -----

    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
