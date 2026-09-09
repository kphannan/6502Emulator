

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionArithmeticTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("ArithmeticUnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionArithmeticTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.writeWord(0xFFFC, 0x2000); // cpu::HardwareVector::RESET
            testMemory.writeWord(0xFFFE, 0xDEAD); // cpu::HardwareVector::IRQ
            testMemory.writeWord(0xFFFA, 0xBEEF); // cpu::HardwareVector::NMI

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

    // Details on how carry and overflow flags work, with examples.
    // http://www.6502.org/tutorials/vflag.html

    // The second purpose was to indicate when the number was outside the (unsigned) range, 0 to 255. But the range of a 8-bit twos complement number is -128 to 127, and the carry does not indicate whether the result is outside this range, as the following examples illustrate:

    //   CLC      ; 1 + 1 = 2, returns C = 0
    //   LDA #$01
    //   ADC #$01

    //   CLC      ; 1 + -1 = 0, returns C = 1
    //   LDA #$01
    //   ADC #$FF

    //   CLC      ; 127 + 1 = 128, returns C = 0
    //   LDA #$7F
    //   ADC #$01

    //   CLC      ; -128 + -1 = -129, returns C = 1
    //   LDA #$80
    //   ADC #$FF
    // This is where V comes in. V indicates whether the result of an addition or subraction is outside the range -128 to 127, i.e. whether there is a twos complement overflow. A few examples are in order:
    //   CLC      ; 1 + 1 = 2, returns V = 0
    //   LDA #$01
    //   ADC #$01

    //   CLC      ; 1 + -1 = 0, returns V = 0
    //   LDA #$01
    //   ADC #$FF

    //   CLC      ; 127 + 1 = 128, returns V = 1
    //   LDA #$7F
    //   ADC #$01

    //   CLC      ; -128 + -1 = -129, returns V = 1
    //   LDA #$80
    //   ADC #$FF

    //   SEC      ; 0 - 1 = -1, returns V = 0
    //   LDA #$00
    //   SBC #$01

    //   SEC      ; -128 - 1 = -129, returns V = 1
    //   LDA #$80
    //   SBC #$01

    //   SEC      ; 127 - -1 = 128, returns V = 1
    //   LDA #$7F
    //   SBC #$FF

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
    // Immediate     ADC #$44      $69  2   2
    TEST_F(InstructionArithmeticTest, ADC_ImmediateUsingCarryClear)
    {
        // --- given
        cpu->A(0x3A);
        cpu->clearC();
        testMemory.write(0x2000, 0x69); // ADC #$7C
        testMemory.write(0x2001, 0x7C);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xB6, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionArithmeticTest, ADC_ImmediateUsingCarrySet)
    {
        // --- given
        cpu->A(0x3A);
        cpu->setC();
        testMemory.write(0x2000, 0x69); // ADC #$7C
        testMemory.write(0x2001, 0x7C);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xB7, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionArithmeticTest, ADC_ImmediateResultsInOverflow)
    {
        //   CLC      ; 127 + 1 = 128, returns C = 0
        //   LDA #$7F
        //   ADC #$01
        //   CLC      ; 127 + 1 = 128, returns V = 1
        //   LDA #$7F
        //   ADC #$01

        // --- given
        cpu->A(0x7F); // Augend
        cpu->clearC();
        testMemory.write(0x2000, 0x69); // ADC #$7C
        testMemory.write(0x2001, 0x01); //   addend

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x80, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    // Zero Page     ADC $44       $65  2   3
    TEST_F(InstructionArithmeticTest, ADC_ZeroPage)
    {
        // --- given
        cpu->A(0x3A);
        cpu->clearC();
        testMemory.write(0x2000, 0x65); // ADC $7C
        testMemory.write(0x2001, 0x44);

        testMemory.write(0x0044, 0x7C);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xB6, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    // Zero Page,X   ADC $44,X     $75  2   4
    TEST_F(InstructionArithmeticTest, ADC_ZeroPageX)
    {
        // --- given
        cpu->A(0x3A);
        cpu->X(0x05);
        cpu->setC();
        testMemory.write(0x2000, 0x75); // ADC $22,X
        testMemory.write(0x2001, 0x22);

        testMemory.write(0x0027, 0x7C);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xB7, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    // Absolute      ADC $4400     $6D  3   4
    TEST_F(InstructionArithmeticTest, ADC_Absolute)
    {
        // --- given
        cpu->A(0x3A);
        cpu->clearC();
        testMemory.write(0x2000, 0x6D); // ADC $DEAD
        testMemory.write(0x2001, 0xAD);
        testMemory.write(0x2002, 0xDE);

        testMemory.write(0xDEAD, 0x7C);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0xB6, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    // Absolute,X    ADC $4400,X   $7D  3   4+
    TEST_F(InstructionArithmeticTest, ADC_AbsoluteX)
    {
        // --- given
        cpu->A(0x3A);
        cpu->X(0x40);
        cpu->clearC();
        testMemory.write(0x2000, 0x7D); // ADC $DEAD,X
        testMemory.write(0x2001, 0xAD);
        testMemory.write(0x2002, 0xDE);

        testMemory.write(0xDEED, 0x7C);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0xB6, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteY $LLHH,Y
    // Absolute,Y    ADC $4400,Y   $79  3   4+
    TEST_F(InstructionArithmeticTest, ADC_AbsoluteY)
    {
        // --- given
        cpu->A(0x3A);
        cpu->X(0x48);
        cpu->Y(0x00);
        cpu->clearC();
        testMemory.write(0x2000, 0x79); // ADC $DEAD,Y
        testMemory.write(0x2001, 0xAD);
        testMemory.write(0x2002, 0xDE);

        testMemory.write(0xDEAD, 0x7B);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0xB5, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    // Indirect,X    ADC ($44,X)   $61  2   6
    TEST_F(InstructionArithmeticTest, ADC_IndirectX)
    {
        // --- given
        cpu->A(0x3A);
        cpu->X(0x31);
        cpu->Y(0x00);
        cpu->clearC();
        testMemory.write(0x2000, 0x61); // ADC ($AD,X)
        testMemory.write(0x2001, 0xAD);

        // table start
        testMemory.write(0x00AD, 0xDE); // 0th entry in lookup table

        testMemory.write(0x00DE, 0xDE); // $31st entry in lookup table
        testMemory.write(0x00DF, 0xFA);

        testMemory.write(0xFADE, 0x7B);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xB5, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- Indirect Indexed Y ($LL),Y
    // Indirect,Y    ADC ($44),Y   $71  2   5+
    TEST_F(InstructionArithmeticTest, ADC_IndirectY)
    {
        // --- given
        cpu->A(0x3A);
        cpu->X(0x31);
        cpu->Y(0x32);
        cpu->clearC();
        testMemory.write(0x2000, 0x71); // ADC ($AD),Y
        testMemory.write(0x2001, 0xAD);

        // Indirection
        testMemory.write(0x00AD, 0xAD); // 0th entry in lookup table
        testMemory.write(0x00AE, 0xDE); // 0th entry in lookup table

        // table start
        testMemory.write(0xDEAD, 0x10); // $32nd entry in lookup table
        testMemory.write(0xDEAE, 0xFB);

        testMemory.write(0xDEDF, 0xEF); // $31st entry in lookup table
        testMemory.write(0xDEE0, 0xBE);

        testMemory.write(0xBEEF, 0x7B);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xB5, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
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
