

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"
#include "InstructionSet.hpp"

namespace m6502
{
    class InstructionLogicalTest : public testing::Test
    {
    public:
        memory::Memory testMemory = *(new memory::Memory( "UnitTest - Logical" ));

        CPU *cpu;

    protected:
        InstructionLogicalTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionLogicalTest() override
        {
            delete cpu;
        }
    };

    // Addressing Modes
    // ----- Immediate #$BB
    // ..... Implied
    // ..... Accumulator
    // ----- ZeroPage $LL
    // ----- ZeroPage,X $LL,X
    // ----- ZeroPage,Y $LL,Y
    // ..... Relative $BB
    // ----- Absolute $LLHH
    // ----- AbsoluteX $LLHH,X
    // ----- AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
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

    //----------------------------------------
    // AND (bitwise AND with accumulator)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     AND #$44      $29  2   2
    // Zero Page     AND $44       $25  2   3
    // Zero Page,X   AND $44,X     $35  2   4
    // Absolute      AND $4400     $2D  3   4
    // Absolute,X    AND $4400,X   $3D  3   4+
    // Absolute,Y    AND $4400,Y   $39  3   4+
    // Indirect,X    AND ($44,X)   $21  2   6
    // Indirect,Y    AND ($44),Y   $31  2   5+
    //
    // + add 1 cycle if page boundary crossed
    //----------------------------------------
    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionLogicalTest, AND_ImmediateZero)
    {
        // --- given
        cpu->A(0x11);
        testMemory.write(0x2000, 0x29); // AND #$00
        testMemory.write(0x2001, 0x00);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLogicalTest, AND_ImmediateFF)
    {
        // --- given
        cpu->A(0x11);
        testMemory.write(0x2000, 0x29); // AND #$FF
        testMemory.write(0x2001, 0xFF);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x11, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLogicalTest, AND_ImmediateMsb)
    {
        // --- given
        cpu->A(0xFF);
        testMemory.write(0x2000, 0x29); // AND #$80
        testMemory.write(0x2001, 0x80);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x80, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionLogicalTest, AND_ZeroPage)
    {
        // --- given
        cpu->A(0xFF);
        testMemory.write(0x2000, 0x25); // AND $14
        testMemory.write(0x2001, 0x14);

        testMemory.write(0x0014, 0xF0);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF0, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionLogicalTest, AND_ZeroPageX)
    {
        // --- given
        cpu->A(0xFF);
        cpu->X(0x04);
        testMemory.write(0x2000, 0x35); // AND $14,X
        testMemory.write(0x2001, 0x14);

        testMemory.write(0x0018, 0x0F);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x0F, cpu->A());
        EXPECT_EQ(0x04, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionLogicalTest, AND_Absolute)
    {
        // --- given
        cpu->A(0xFF);
        testMemory.write(0x2000, 0x2D); // AND $2345
        testMemory.write(0x2001, 0x45);
        testMemory.write(0x2002, 0x23);

        testMemory.write(0x2345, 0x73);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x73, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x73, testMemory.read(0x2345));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionLogicalTest, AND_AbsoluteX)
    {
        // --- given
        cpu->A(0x93);
        cpu->X(0x01);
        testMemory.write(0x2000, 0x3D); // AND $2345,X
        testMemory.write(0x2001, 0x45);
        testMemory.write(0x2002, 0x23);

        testMemory.write(0x2346, 0x72);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x12, cpu->A());
        EXPECT_EQ(0x01, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x72, testMemory.read(0x2346));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionLogicalTest, AND_AbsoluteY)
    {
        // --- given
        cpu->A(0x93);
        cpu->Y(0x02);
        testMemory.write(0x2000, 0x39); // AND $2345,Y
        testMemory.write(0x2001, 0x44);
        testMemory.write(0x2002, 0x23);

        testMemory.write(0x2346, 0x72);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x12, cpu->A());
        EXPECT_EQ(0x02, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x72, testMemory.read(0x2346));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionLogicalTest, AND_IndexedIndirectX)
    {
        // --- given
        cpu->A(0x6F);
        cpu->X(0x16);
        testMemory.write(0x2002, 0x21); // AND ($60,X)
        testMemory.write(0x2003, 0x60); // offset from base address

        // Lookup table of addresses
        testMemory.write(0x0076, 0x23); // Entry 0, $LL Address lookup table
        testMemory.write(0x0077, 0x30); //          $HH

        testMemory.write(0x3023, 0x41); // Data

        // --- when
        cpu->executeFromAddress(0x2002, 1);

        // --- then
        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x41, cpu->A());
        EXPECT_EQ(0x16, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionLogicalTest, AND_IndirectIndexedY)
    {
        // --- given
        cpu->A(0xFF);
        cpu->Y(0x16);
        testMemory.write(0x2002, 0x31); // AND ($60),Y
        testMemory.write(0x2003, 0x60); //  low address of indirect

        // Indirect address
        testMemory.write(0x0060, 0x00); // Entry 0, $LL Address lookup table
        testMemory.write(0x0061, 0x70); //          $HH

        // Lookup table of addresses
        testMemory.write(0x7016, 0x23); // Entry 0, $LL Address lookup table
        testMemory.write(0x7017, 0x30); //          $HH

        testMemory.write(0x3023, 0x41); // Data

        // --- when
        cpu->executeFromAddress(0x2002, 1);

        // --- then
        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x41, cpu->A());
        EXPECT_EQ(0x16, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    //----------------------------------------
    // BIT (test BITs)
    //
    // Affects Flags: N V Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Zero Page     BIT $44       $24  2   3
    // Absolute      BIT $4400     $2C  3   4
    //
    // BIT sets the Z flag as though the value in the address tested were ANDed with the accumulator. The N and V flags are set to match bits 7 and 6 respectively in the value stored at the tested address.
    // BIT is often used to skip one or two following bytes as in:
    //
    // CLOSE1 LDX #$10   If entered here, we
    //        .BYTE $2C  effectively perform
    // CLOSE2 LDX #$20   a BIT test on $20A2,
    //        .BYTE $2C  another one on $30A2,
    // CLOSE3 LDX #$30   and end up with the X
    // CLOSEX LDA #12    register still at $10
    //        STA ICCOM,X upon arrival here.
    //
    // Beware: a BIT instruction used in this way as a NOP does have effects: the flags may be modified, and the read of the absolute address, if it happens to access an I/O device, may cause an unwanted action.
    //----------------------------------------
    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionLogicalTest, BIT_ZeroPage)
    {
        // --- given
        cpu->A(0x26);
        testMemory.write(0x2000, 0x24); // EOR #$00
        testMemory.write(0x2001, 0x32);

        testMemory.write(0x0032, 0xD9);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x26, cpu->A());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
        // ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... ZeroPage,X $LL,X
    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionLogicalTest, BIT_Absolute)
    {
        // --- given
        cpu->A(0x99);
        testMemory.write(0x2000, 0x2C); // EOR #$4334
        testMemory.write(0x2001, 0x34);
        testMemory.write(0x2002, 0x43);

        testMemory.write(0x4334, 0xA2);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x99, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
        // ADD_FAILURE_AT(__FILE__, __LINE__);
    }

    // ..... AbsoluteX $LLHH,X
    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // EOR (bitwise Exclusive OR)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     EOR #$44      $49  2   2
    // Zero Page     EOR $44       $45  2   3
    // Zero Page,X   EOR $44,X     $55  2   4
    // Absolute      EOR $4400     $4D  3   4
    // Absolute,X    EOR $4400,X   $5D  3   4+
    // Absolute,Y    EOR $4400,Y   $59  3   4+
    // Indirect,X    EOR ($44,X)   $41  2   6
    // Indirect,Y    EOR ($44),Y   $51  2   5+
    //
    // + add 1 cycle if page boundary crossed
    //----------------------------------------
    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionLogicalTest, EOR_Immediate)
    {
        // --- given
        cpu->A(0x66);
        testMemory.write(0x2000, 0x49); // EOR #$66
        testMemory.write(0x2001, 0x66);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLogicalTest, EOR_ImmediateZero)
    {
        // --- given
        cpu->A(0x66);
        testMemory.write(0x2000, 0x49); // EOR #$00
        testMemory.write(0x2001, 0x00);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x66, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionLogicalTest, EOR_ImmediateFF)
    {
        // --- given
        cpu->A(0x66);
        testMemory.write(0x2000, 0x49); // EOR #$FF
        testMemory.write(0x2001, 0xFF);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x99, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionLogicalTest, EOR_ZeroPage)
    {
        // --- given
        cpu->A(0xFF);
        testMemory.write(0x2000, 0x45); // EOR $14
        testMemory.write(0x2001, 0x14);

        testMemory.write(0x0014, 0xF0);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x0F, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionLogicalTest, EOR_ZeroPageX)
    {
        // --- given
        cpu->A(0xFF);
        cpu->X(0x04);
        testMemory.write(0x2000, 0x55); // EOR $14,X
        testMemory.write(0x2001, 0x14);

        testMemory.write(0x0018, 0x0F);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF0, cpu->A());
        EXPECT_EQ(0x04, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionLogicalTest, EOR_Absolute)
    {
        // --- given
        cpu->A(0xFF);
        testMemory.write(0x2000, 0x4D); // EOR $2345
        testMemory.write(0x2001, 0x45);
        testMemory.write(0x2002, 0x23);

        testMemory.write(0x2345, 0x73);
        //   1111 1111
        //   0111 0011
        // ^ 1000 1100

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x8C, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0x73, testMemory.read(0x2345));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionLogicalTest, EOR_AbsoluteX)
    {
        // --- given
        cpu->A(0x93);
        cpu->X(0x01);
        testMemory.write(0x2000, 0x5D); // EOR $2345,X
        testMemory.write(0x2001, 0x45);
        testMemory.write(0x2002, 0x23);

        testMemory.write(0x2346, 0x72);
        //   1001 0011  $93
        //   0111 0010  $72
        // ^ 1110 0001

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0xE1, cpu->A());
        EXPECT_EQ(0x01, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0x72, testMemory.read(0x2346));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionLogicalTest, EOR_AbsoluteY)
    {
        // --- given
        cpu->A(0x93);
        cpu->X(0x01);
        cpu->Y(0x02);
        testMemory.write(0x2000, 0x59); // EOR $2345,Y
        testMemory.write(0x2001, 0x44);
        testMemory.write(0x2002, 0x23);

        testMemory.write(0x2346, 0x93);
        //   1001 0011
        //   1001 0011
        // ^ 0000 0000

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0x01, cpu->X());
        EXPECT_EQ(0x02, cpu->Y());
        EXPECT_EQ(0b00100010, cpu->P());
        EXPECT_EQ(0x93, testMemory.read(0x2346));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionLogicalTest, EOR_IndexedIndirectX)
    {
        // --- given
        cpu->A(0xAA);
        cpu->X(0x14);
        testMemory.write(0x2002, 0x41); // EOR ($70,X)
        testMemory.write(0x2003, 0x70); // offset from base address

        // Lookup table of addresses
        testMemory.write(0x0084, 0x22); // Entry 0, $LL Address lookup table
        testMemory.write(0x0085, 0x40); //          $HH

        testMemory.write(0x4022, 0xFF); // Data

        // --- when
        cpu->executeFromAddress(0x2002, 1);

        // --- then
        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x55, cpu->A());
        EXPECT_EQ(0x14, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionLogicalTest, EOR_IndirectIndexedY)
    {
        // --- given
        cpu->A(0x08);
        cpu->Y(0x08);
        testMemory.write(0x2002, 0x51); // EOR ($70),Y      ; Indirect table address
        testMemory.write(0x2003, 0x70);

        // Indirect
        testMemory.write(0x0070, 0x40); // $LL Address lookup table
        testMemory.write(0x0071, 0x35); // $HH

        // Lookup table (only 8th entry)
        testMemory.write(0x3548, 0x40); // Entry $08, $LL Address lookup table
        testMemory.write(0x3549, 0xF0); //            $HH

        testMemory.write(0xF040, 0x23); // Data

        // --- when
        cpu->executeFromAddress(0x2002, 1);

        // --- then
        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x2B, cpu->A());
        EXPECT_EQ(0x08, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    //----------------------------------------
    // ORA (bitwise OR with Accumulator)
    //
    // Affects Flags: N Z
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Immediate     ORA #$44      $09  2   2
    // Zero Page     ORA $44       $05  2   3
    // Zero Page,X   ORA $44,X     $15  2   4
    // Absolute      ORA $4400     $0D  3   4
    // Absolute,X    ORA $4400,X   $1D  3   4+
    // Absolute,Y    ORA $4400,Y   $19  3   4+
    // Indirect,X    ORA ($44,X)   $01  2   6
    // Indirect,Y    ORA ($44),Y   $11  2   5+
    //
    // + add 1 cycle if page boundary crossed
    //----------------------------------------
    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(InstructionLogicalTest, ORA_Immediate)
    {
        // --- given
        cpu->A(0x11);
        testMemory.write(0x2000, 0x09); // ORA #$22
        testMemory.write(0x2001, 0x22);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x33, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(InstructionLogicalTest, ORA_ZeroPage)
    {
        // --- given
        cpu->A(0x21);
        testMemory.write(0x2000, 0x05); // ORA $04
        testMemory.write(0x2001, 0x04);

        testMemory.write(0x0004, 0x26);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x27, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionLogicalTest, ORA_ZeroPageX)
    {
        // --- given
        cpu->A(0x84);
        cpu->X(0x05);
        testMemory.write(0x2000, 0x15); // ORA $04,X
        testMemory.write(0x2001, 0x04);

        testMemory.write(0x0009, 0x71);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF5, cpu->A());
        EXPECT_EQ(0x05, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionLogicalTest, ORA_Absolute)
    {
        // --- given
        cpu->A(0x21);
        testMemory.write(0x2000, 0x0D); // ORA $4024
        testMemory.write(0x2001, 0x24);
        testMemory.write(0x2002, 0x40);

        testMemory.write(0x4024, 0x48);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x69, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionLogicalTest, ORA_AbsoluteX)
    {
        // --- given
        cpu->A(0x08);
        cpu->X(0x08);
        testMemory.write(0x2000, 0x1D); // ORA $3010,X
        testMemory.write(0x2001, 0x10);
        testMemory.write(0x2002, 0x30);

        testMemory.write(0x3018, 0x80);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x88, cpu->A());
        EXPECT_EQ(0x08, cpu->X());
        // EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(InstructionLogicalTest, ORA_AbsoluteY)
    {
        // --- given
        cpu->A(0x07);
        cpu->Y(0x04);
        testMemory.write(0x2000, 0x19); // ORA $3012,Y
        testMemory.write(0x2001, 0x12);
        testMemory.write(0x2002, 0x30);

        testMemory.write(0x3016, 0x70);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x77, cpu->A());
        EXPECT_EQ(0x04, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ..... Indirect ($LLHH)

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(InstructionLogicalTest, ORA_IndirectX)
    {
        // --- given
        cpu->A(0x08);
        cpu->X(0x06);
        testMemory.write(0x2002, 0x01); // ORA ($70,X)
        testMemory.write(0x2003, 0x70); // offset from base address

        // Lookup table of addresses
        testMemory.write(0x0076, 0x23); // Entry 0, $LL Address lookup table
        testMemory.write(0x0077, 0x30); //          $HH

        testMemory.write(0x3023, 0x41); // Data

        // --- when
        cpu->executeFromAddress(0x2002, 1);

        // --- then
        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x49, cpu->A());
        EXPECT_EQ(0x06, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(InstructionLogicalTest, ORA_IndirectY)
    {
        // --- given
        cpu->A(0x08);
        cpu->Y(0x10);
        testMemory.write(0x2002, 0x11); // ORA ($70),Y      ; Indirect table address
        testMemory.write(0x2003, 0x70);

        // Lookup table of addresses
        testMemory.write(0x0070, 0x00); // Entry $10, $LL Address lookup table
        testMemory.write(0x0071, 0xE1); //            $HH

        testMemory.write(0xE110, 0x40); // Entry $10, $LL Address lookup table
        testMemory.write(0xE111, 0x35); //            $HH

        testMemory.write(0x3540, 0x23); // Data
        //   0000 1000
        //   0010 0011
        // | 0010 1011

        // --- when
        cpu->executeFromAddress(0x2002, 1);

        // --- then
        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x2B, cpu->A());
        EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- Shift & Rotate () -----
    // ----- Flag () -----
    // ----- Comparison () -----
    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
