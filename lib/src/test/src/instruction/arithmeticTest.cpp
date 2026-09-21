

#include <gtest/gtest.h>

#include "6502.hpp"
#include "InstructionSet.hpp"
#include "memory.hpp"

namespace m6502 {
    class InstructionArithmeticTest : public testing::Test {
    public:
            memory::Memory testMemory = *(new memory::Memory( "UnitTest - Arithmetic" ));

            CPU* cpu;

    protected:
        InstructionArithmeticTest() {
                // Destination of the reset vector - leaves zeroPage available for
                // testing
                testMemory[0x2000] = 0x49;  // LDA #00 // starting instruction after reset
                testMemory[0x2001] = 0x5A;

                // Reset vector points to start of memory
                testMemory.writeWord(0xFFFC, 0x2000);  // cpu::HardwareVector::RESET
                testMemory.writeWord(0xFFFE, 0xDEAD);  // cpu::HardwareVector::IRQ
                testMemory.writeWord(0xFFFA, 0xBEEF);  // cpu::HardwareVector::NMI

                cpu = new CPU(testMemory);
            }

            ~InstructionArithmeticTest() override { delete cpu; }
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

    // The second purpose was to indicate when the number was outside the (unsigned)
    // range, 0 to 255. But the range of a 8-bit twos complement number is -128 to
    // 127, and the carry does not indicate whether the result is outside this
    // range, as the following examples illustrate:

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
    // This is where V comes in. V indicates whether the result of an addition or
    // subraction is outside the range -128 to 127, i.e. whether there is a twos
    // complement overflow. A few examples are in order:
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
    // ADC results are dependant on the setting of the decimal flag. In decimal
    // mode, addition is carried out on the assumption that the values involved are
    // packed BCD (Binary Coded Decimal). There is no way to add without carry.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB
    // Immediate     ADC #$44      $69  2   2
    TEST_F(InstructionArithmeticTest, ADC_ImmediateUsingCarryClear) {
        // --- given
        cpu->A(0x3A);
        cpu->clearC();
        testMemory[0x2000] = 0x69;  // ADC #$7C
        testMemory[0x2001] = 0x7C;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xB6, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionArithmeticTest, ADC_ImmediateUsingCarrySet) {
        // --- given
        cpu->A(0x3A);
        cpu->setC();
        testMemory[0x2000] = 0x69;  // ADC #$7C
        testMemory[0x2001] = 0x7C;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xB7, cpu->A());
        EXPECT_EQ(0b11100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionArithmeticTest, ADC_ImmediateResultsInOverflow) {
        //   CLC      ; 127 + 1 = 128, returns C = 0
        //   LDA #$7F
        //   ADC #$01
        //   CLC      ; 127 + 1 = 128, returns V = 1
        //   LDA #$7F
        //   ADC #$01

        // --- given
        cpu->A(0x7F);  // Augend
        cpu->clearC();
        testMemory[0x2000] = 0x69;  // ADC #$7C
        testMemory[0x2001] = 0x01;  //   addend

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
    TEST_F(InstructionArithmeticTest, ADC_ZeroPage) {
        // --- given
        cpu->A(0x3A);
        cpu->clearC();
        testMemory[0x2000] = 0x65;  // ADC $7C
        testMemory[0x2001] = 0x44;

        testMemory[0x0044] = 0x7C;

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
    TEST_F(InstructionArithmeticTest, ADC_ZeroPageX) {
        // --- given
        cpu->A(0x3A);
        cpu->X(0x05);
        cpu->setC();
        testMemory[0x2000] = 0x75;  // ADC $22,X
        testMemory[0x2001] = 0x22;

        testMemory[0x0027] = 0x7C;

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
    TEST_F(InstructionArithmeticTest, ADC_Absolute) {
        // --- given
        cpu->A(0x3A);
        cpu->clearC();
        testMemory[0x2000] = 0x6D;  // ADC $DEAD
        testMemory[0x2001] = 0xAD;
        testMemory[0x2002] = 0xDE;

        testMemory[0xDEAD] = 0x7C;

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
    TEST_F(InstructionArithmeticTest, ADC_AbsoluteX) {
        // --- given
        cpu->A(0x3A);
        cpu->X(0x40);
        cpu->clearC();
        testMemory[0x2000] = 0x7D;  // ADC $DEAD,X
        testMemory[0x2001] = 0xAD;
        testMemory[0x2002] = 0xDE;

        testMemory[0xDEED] = 0x7C;

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
    TEST_F(InstructionArithmeticTest, ADC_AbsoluteY) {
        // --- given
        cpu->A(0x3A);
        cpu->X(0x48);
        cpu->Y(0x00);
        cpu->clearC();
        testMemory[0x2000] = 0x79;  // ADC $DEAD,Y
        testMemory[0x2001] = 0xAD;
        testMemory[0x2002] = 0xDE;

        testMemory[0xDEAD] = 0x7B;

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
    TEST_F(InstructionArithmeticTest, ADC_IndirectX) {
        // --- given
        cpu->A(0x3A);
        cpu->X(0x31);
        cpu->Y(0x00);
        cpu->clearC();
        testMemory[0x2000] = 0x61;  // ADC ($AD,X)
        testMemory[0x2001] = 0xAD;

        // table start
        testMemory[0x00AD] = 0xDE;  // 0th entry in lookup table

        testMemory[0x00DE] = 0xDE;  // $31st entry in lookup table
        testMemory[0x00DF] = 0xFA;

        testMemory[0xFADE] = 0x7B;

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
    TEST_F(InstructionArithmeticTest, ADC_IndirectY) {
        // --- given
        cpu->A(0x3A);
        cpu->X(0x31);
        cpu->Y(0x32);
        cpu->clearC();
        testMemory[0x2000] = 0x71;  // ADC ($AD),Y
        testMemory[0x2001] = 0xAD;

        // Indirection
        testMemory[0x00AD] = 0xAD;  // 0th entry in lookup table
        testMemory[0x00AE] = 0xDE;  // 0th entry in lookup table

        // table start
        testMemory[0xDEAD] = 0x10;  // $32nd entry in lookup table
        testMemory[0xDEAE] = 0xFB;

        testMemory[0xDEDF] = 0xEF;  // $31st entry in lookup table
        testMemory[0xDEE0] = 0xBE;

        testMemory[0xBEEF] = 0x7B;

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
    // SBC results are dependant on the setting of the decimal flag. In decimal
    // mode, subtraction is carried out on the assumption that the values involved
    // are packed BCD (Binary Coded Decimal). There is no way to subtract without
    // the carry which works as an inverse borrow. i.e, to subtract you set the
    // carry before the operation. If the carry is cleared by the operation, it
    // indicates a borrow occurred.
    //----------------------------------------

    // Addressing Modes
    // ----- Immediate #$BB     ($E9)
    TEST_F(InstructionArithmeticTest, SBC_Immediate_positive) {
        // --- given
        cpu->A(0x65);
        cpu->setC();
        testMemory[0x2000] = 0xE9;  // SBC #$64
        testMemory[0x2001] = 0x64;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x01, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionArithmeticTest, SBC_Immediate_negative) {
        // --- given
        cpu->A(0x64);
        cpu->setC();
        testMemory[0x2000] = 0xE9;  // SBC #$64
        testMemory[0x2001] = 0x65;

        // testMemory[0x0027] = 0x7C;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xFF, cpu->A());
        EXPECT_EQ(0b10100001, cpu->P());

        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL       ($E5)
    TEST_F(InstructionArithmeticTest, SBC_ZeroPage) {
        // x64     0110 0100       100
        // x7C     0111 1100       124
        // x7C^    1000 0011 + 1
        //         1000 0100
        //
        //       + 0110 0100
        //       1 1110 1000
        // --- given
        cpu->A(0x64);
        cpu->X(0x05);
        cpu->setC();
        testMemory[0x2000] = 0xE5;  // SBC $64
        testMemory[0x2001] = 0x44;

        testMemory[0x0044] = 0x7C;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xE8, cpu->A());
        EXPECT_EQ(0b10100001, cpu->P());

        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X   ($F5)
    TEST_F(InstructionArithmeticTest, SBC_ZeroPageX) {
        // --- given
        cpu->A(0x77);
        cpu->X(0x05);
        cpu->setC();
        testMemory[0x2000] = 0xF5;  // SBC $64
        testMemory[0x2001] = 0x44;

        testMemory[0x0044] = 0x7C;
        testMemory[0x0049] = 0x20;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x57, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH     ($ED)
    TEST_F(InstructionArithmeticTest, SBC_Absolute) {
        // --- given
        cpu->A(0x70);
        cpu->setC();
        testMemory[0x2000] = 0xED;  // SBC $1234
        testMemory[0x2001] = 0x34;
        testMemory[0x2002] = 0x12;

        testMemory[0x1234] = 0x60;
        // testMemory[0x0049] = 0x20;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x10, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X  ($FD)
    TEST_F(InstructionArithmeticTest, SBC_AbsoluteX) {
        cpu->A(0x77);
        cpu->X(0x05);
        cpu->setC();
        testMemory[0x2000] = 0xFD;  // SBC $1234,X
        testMemory[0x2001] = 0x34;
        testMemory[0x2002] = 0x12;

        testMemory[0x1239] = 0x22;
        //    testMemory[0x0049] = 0x20;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x55, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteY $LLHH,Y  ($F9)
    TEST_F(InstructionArithmeticTest, SBC_AbsoluteY) {
        cpu->A(0x77);
        cpu->Y(0x010);
        cpu->setC();
        testMemory[0x2000] = 0xF9;  // SBC $1234,Y
        testMemory[0x2001] = 0x34;
        testMemory[0x2002] = 0x12;

        //    testMemory[0x0044] = 0x7C;
        //    testMemory[0x0049] = 0x20;
        testMemory[0x1244] = 0x57;
        testMemory[0x1245] = 0x20;

        // testMemory[] =

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x20, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)     ($E1)
    TEST_F(InstructionArithmeticTest, SBC_IndirectX) {
        cpu->A(0x45);
        cpu->X(0x05);
        cpu->setC();
        testMemory[0x2000] = 0xE1;  // SBC ($44,X)
        testMemory[0x2001] = 0x44;

        // Address: 0x44 + 0x05 = 0x49

        testMemory[0x0049] = 0x7C;  // lsb
        testMemory[0x004A] = 0x20;  // msb

        testMemory[0x207C] = 0x10;   // table base
        testMemory[0x207D] = 0x11;   // [1]
        testMemory[0x207E] = 0x12;   // [2]
        testMemory[0x207F] = 0x13;   // [3]
        testMemory[0x2080] = 0x14;   // [4]
        testMemory[0x2081] = 0x15;   // [5]


        // --- when
        // 0x45 - 0x10 = 0x35
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x35, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- Indirect Indexed Y ($LL),Y     ($F1)
    TEST_F(InstructionArithmeticTest, SBC_IndirectY) {
        cpu->A(0x77);
        cpu->Y(0x10);
        cpu->setC();
        testMemory[0x2000] = 0xF1;  // SBC $1234
        testMemory[0x2001] = 0x44;

        testMemory[0x0044] = 0x60;  // pointer to Table base
        testMemory[0x0045] = 0x20;

        testMemory[0x2060] = 0x20;  // Table base
        testMemory[0x2061] = 0x20;  // [1]
        testMemory[0x2062] = 0x20;  // [1]
        testMemory[0x2063] = 0x20;  // [2]
        testMemory[0x2064] = 0x20;  // [2]
        testMemory[0x2065] = 0x00;  // [3]
        testMemory[0x2066] = 0x80;  // [3]

        testMemory[0x2070] = 0x00;  // [10]
        testMemory[0x2071] = 0x80;  // [10]


        testMemory[0x8000] = 0x07;

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x70, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());

        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- Logical () -----
    // ----- Shift & Rotate () -----
    // ----- Flag () -----
    // ----- Comparison () -----

    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}  // namespace m6502
