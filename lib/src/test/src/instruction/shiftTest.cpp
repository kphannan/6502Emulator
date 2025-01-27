

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class InstructionShiftTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("ShiftUnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        InstructionShiftTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A);

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~InstructionShiftTest() override
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

    //----------------------------------------
    // ASL (Arithmetic Shift Left)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   ASL A         $0A  1   2
    // Zero Page     ASL $44       $06  2   5
    // Zero Page,X   ASL $44,X     $16  2   6
    // Absolute      ASL $4400     $0E  3   6
    // Absolute,X    ASL $4400,X   $1E  3   7
    //
    // ASL shifts all bits left one position. 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied

    // ----- Accumulator
    TEST_F(InstructionShiftTest, ASL_Accumulator)
    {
        // --- given
        cpu->A(0x22); // 0010 0010 -> 0100 0100 C:0
        cpu->setC();
        testMemory.write(0x2000, 0x0A); // ASL A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x44, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, ASL_AccumulatorZero)
    {
        // --- given
        cpu->A(0x80);
        cpu->clearC();
        testMemory.write(0x2000, 0x0A); // ASL A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100011, cpu->P());
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // TODO add case for negative result

    // ----- ZeroPage $LL
    TEST_F(InstructionShiftTest, ASL_ZeroPage)
    {
        // --- given
        cpu->A(0x22); // 0010 0010 -> 0100 0100 C:0
        cpu->setC();
        testMemory.write(0x2000, 0x06); // ASL $nn
        testMemory.write(0x2001, 0x08);

        testMemory.write(0x0008, 0x22);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x22, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x44, testMemory.read(0x0008));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, ASL_ZeroPageZero)
    {
        // --- given
        cpu->A(0x80);
        cpu->clearC();
        testMemory.write(0x2000, 0x06); // ASL $nn
        testMemory.write(0x2001, 0x08);

        testMemory.write(0x0008, 0x80);

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x80, cpu->A());
        EXPECT_EQ(0b00100011, cpu->P());
        EXPECT_EQ(0x00, testMemory.read(0x0008));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionShiftTest, ASL_ZeroPageXNegative)
    {
        // --- given
        cpu->A(0x22);
        cpu->X(0x10);
        cpu->setC();
        testMemory.write(0x2000, 0x16); // ASL $nn,X
        testMemory.write(0x2001, 0x08);

        testMemory.write(0x0018, 0x42); // 0100 0010 -> 1000 0100

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x22, cpu->A());
        EXPECT_EQ(0x10, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0x84, testMemory.read(0x0018));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionShiftTest, ASL_Absolute)
    {
        // --- given
        cpu->A(0x22); // 0010 0010 -> 0100 0100 C:0
        cpu->setC();
        testMemory.write(0x2000, 0x0E); // ASL $nnnn
        testMemory.write(0x2001, 0x08);
        testMemory.write(0x2002, 0x22);

        testMemory.write(0x2208, 0x22); // 0010 0010 -> 0100 0100

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x22, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x44, testMemory.read(0x2208));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionShiftTest, ASL_AbsoluteX)
    {
        // --- given
        cpu->A(0x22);
        cpu->X(0x10);
        cpu->setC();
        testMemory.write(0x2000, 0x1E); // ASL $nnnn,X
        testMemory.write(0x2001, 0x08);
        testMemory.write(0x2002, 0x08);

        testMemory.write(0x0818, 0x42); // 0100 0010 -> 1000 0100

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x22, cpu->A());
        EXPECT_EQ(0x10, cpu->X());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0x84, testMemory.read(0x0818));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // LSR (Logical Shift Right)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   LSR A         $4A  1   2
    // Zero Page     LSR $44       $46  2   5
    // Zero Page,X   LSR $44,X     $56  2   6
    // Absolute      LSR $4400     $4E  3   6
    // Absolute,X    LSR $4400,X   $5E  3   7
    //
    // LSR shifts all bits right one position. 0 is shifted into bit 7 and the original bit 0 is shifted into the Carry.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied

    // ----- Accumulator
    TEST_F(InstructionShiftTest, LSR_Accumulator)
    {
        // --- given
        cpu->A(0xF1);
        cpu->clearC();
        testMemory.write(0x2000, 0x4A); // LSR A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x78, cpu->A());
        EXPECT_EQ(0b00100001, cpu->P());
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, LSR_AccumulatorZero)
    {
        // --- given
        cpu->A(0x01);
        cpu->clearC();
        testMemory.write(0x2000, 0x4A); // LSR A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100011, cpu->P());
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage $LL
    TEST_F(InstructionShiftTest, LSR_ZeroPage)
    {
        // --- given
        cpu->A(0xF1);
        cpu->setC();
        testMemory.write(0x2000, 0x46); // LSR $nn
        testMemory.write(0x2001, 0x56);

        testMemory.write(0x0056, 0x56); // Data: 0101 0110 -> 0010 1011 C:0
        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF1, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x2B, testMemory.read(0x0056));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, LSR_ZeroPageZero)
    {
        // --- given
        cpu->A(0x01);
        cpu->clearC();
        testMemory.write(0x2000, 0x46); // LSR $nn
        testMemory.write(0x2001, 0x50);

        testMemory.write(0x0050, 0x01); // Data: 0101 0110 -> 0010 1011 C:0

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x01, cpu->A());
        EXPECT_EQ(0b00100011, cpu->P());
        EXPECT_EQ(0x00, testMemory.read(0x0050));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionShiftTest, LSR_ZeroPageX)
    {
        // --- given
        cpu->A(0x01);
        cpu->X(0x15);
        cpu->clearC();
        testMemory.write(0x2000, 0x56); // LSR $nn,X
        testMemory.write(0x2001, 0x50);

        testMemory.write(0x0065, 0x01); // Data: 0101 0110 -> 0010 1011 C:0

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0x01, cpu->A());
        EXPECT_EQ(0x15, cpu->X());
        EXPECT_EQ(0b00100011, cpu->P());
        EXPECT_EQ(0x00, testMemory.read(0x0065));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionShiftTest, LSR_Absolute)
    {
        // --- given
        cpu->A(0x01);
        cpu->X(0x15);
        cpu->setC();
        testMemory.write(0x2000, 0x4E); // LSR $nnnn
        testMemory.write(0x2001, 0x50);
        testMemory.write(0x2002, 0x50);

        testMemory.write(0x5050, 0x92); // Data: 1001 0010 -> 0100 1001 C:0

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x01, cpu->A());
        EXPECT_EQ(0x15, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x49, testMemory.read(0x5050));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionShiftTest, LSR_AbsoluteX)
    {
        // --- given
        cpu->A(0x01);
        cpu->X(0x15);
        cpu->setC();
        testMemory.write(0x2000, 0x5E); // LSR $nnnn,X
        testMemory.write(0x2001, 0x50);
        testMemory.write(0x2002, 0x50);

        testMemory.write(0x5065, 0xD2); // Data: 1101 0010 -> 0100 1001 C:0

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0x01, cpu->A());
        EXPECT_EQ(0x15, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x69, testMemory.read(0x5065));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // ROL (ROtate Left)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   ROL A         $2A  1   2
    // Zero Page     ROL $44       $26  2   5
    // Zero Page,X   ROL $44,X     $36  2   6
    // Absolute      ROL $4400     $2E  3   6
    // Absolute,X    ROL $4400,X   $3E  3   7
    //
    // ROL shifts all bits left one position. The Carry is shifted into bit 0 and the original bit 7 is shifted into the Carry.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied

    // ----- Accumulator
    TEST_F(InstructionShiftTest, ROL_AccumulatorWithCarryMsb0)
    {
        // Verify carry of zero msb to carry flag

        // --- given
        cpu->A(0x7F); // 0b0111 1111  -> 1111 1111  C:0
        cpu->setC();
        testMemory.write(0x2000, 0x2A); // ROL A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0xFF, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, ROL_AccumulatorWithCarryMsb1)
    {
        // Verify carry of one lsb to carry flag

        // --- given
        cpu->A(0x94); // 0b1001 0100  -> 0010 1001  C:1
        cpu->setC();
        testMemory.write(0x2000, 0x2A); // ROL A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x29, cpu->A());
        EXPECT_EQ(0b00100001, cpu->P());
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, ROL_AccumulatorZero)
    {
        // Verify zero flag set

        // --- given
        cpu->A(0x80); // 0b1000 0000 -> 0000 0000  C:1
        cpu->clearC();
        testMemory.write(0x2000, 0x2A); // ROL A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100011, cpu->P());
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, ROL_AccumulatorNegative)
    {
        // Verify negative flag set when Carry flag is set

        // --- given
        cpu->A(0xC0); // 0b1100 0000 -> 1000 0000  C:1
        cpu->clearC();
        testMemory.write(0x2000, 0x2A); // ROL A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x80, cpu->A());
        EXPECT_EQ(0b10100001, cpu->P());
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage $LL
    TEST_F(InstructionShiftTest, ROL_ZeroPage)
    {
        // --- given
        cpu->A(0xC0);
        cpu->clearC();
        testMemory.write(0x2000, 0x26); // ROL $12
        testMemory.write(0x2001, 0x12);

        testMemory.write(0x0012, 0xC0); // Data: 0b1100 0000 -> 1000 0000  C:1

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xC0, cpu->A());
        EXPECT_EQ(0b10100001, cpu->P());
        EXPECT_EQ(0x80, testMemory.read(0x0012));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionShiftTest, ROL_ZeroPageX)
    {
        // --- given
        cpu->A(0xC0);
        cpu->X(0x01);
        cpu->setC();
        testMemory.write(0x2000, 0x36); // ROL $12,X
        testMemory.write(0x2001, 0x12);

        testMemory.write(0x0013, 0xE0); // Data: 0b1110 0000 -> 1100 0001  C:1

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xC0, cpu->A());
        EXPECT_EQ(0x01, cpu->X());
        EXPECT_EQ(0b10100001, cpu->P());
        EXPECT_EQ(0xC1, testMemory.read(0x0013));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionShiftTest, ROL_Absolute)
    {
        // --- given
        cpu->A(0xC0);
        cpu->X(0x01);
        cpu->setC();
        testMemory.write(0x2000, 0x2E); // ROL $1234
        testMemory.write(0x2001, 0x34);
        testMemory.write(0x2002, 0x12);

        testMemory.write(0x1234, 0xE0); // Data: 0b1110 0000 -> 1100 0001  C:1

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0xC0, cpu->A());
        EXPECT_EQ(0x01, cpu->X());
        EXPECT_EQ(0b10100001, cpu->P());
        EXPECT_EQ(0xC1, testMemory.read(0x1234));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionShiftTest, ROL_AbsoluteX)
    {
        // --- given
        cpu->A(0xC0);
        cpu->X(0x01);
        cpu->setC();
        testMemory.write(0x2000, 0x3E); // ROL $1243,X
        testMemory.write(0x2001, 0x43);
        testMemory.write(0x2002, 0x12);

        testMemory.write(0x1244, 0xE0); // Data: 0b1110 0000 -> 1100 0001  C:1

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0xC0, cpu->A());
        EXPECT_EQ(0x01, cpu->X());
        EXPECT_EQ(0b10100001, cpu->P());
        EXPECT_EQ(0xC1, testMemory.read(0x1244));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    //----------------------------------------
    // ROR (ROtate Right)
    //
    // Affects Flags: N Z C
    //
    // MODE           SYNTAX       HEX LEN TIM
    // Accumulator   ROR A         $6A  1   2
    // Zero Page     ROR $44       $66  2   5
    // Zero Page,X   ROR $44,X     $76  2   6
    // Absolute      ROR $4400     $6E  3   6
    // Absolute,X    ROR $4400,X   $7E  3   7
    //
    // ROR shifts all bits right one position. The Carry is shifted into bit 7 and the original bit 0 is shifted into the Carry.
    //----------------------------------------

    // Addressing Modes
    // ..... Immediate #$BB
    // ..... Implied

    // ----- Accumulator
    TEST_F(InstructionShiftTest, ROR_AccumulatorWithCarryLsb1)
    {
        // --- given
        cpu->A(0xF1);
        cpu->setC();
        testMemory.write(0x2000, 0x6A); // ROR A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0xF8, cpu->A());
        // EXPECT_EQ(0xF0, cpu->S());
        EXPECT_EQ(0b10100001, cpu->P());
        // EXPECT_EQ(0x09, testMemory.read(0x7235));
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, ROR_AccumulatorWithCarryLsb0)
    {
        // --- given
        cpu->A(0xF0);
        cpu->setC();
        testMemory.write(0x2000, 0x6A); // ROR A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0xF8, cpu->A());
        // EXPECT_EQ(0xF0, cpu->S());
        EXPECT_EQ(0b10100000, cpu->P());
        // EXPECT_EQ(0x09, testMemory.read(0x7235));
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, ROR_AccumulatorWithoutCarryLsb1)
    {
        // --- given
        cpu->A(0xF1); // 0b1111 0001  -> 0111 1000  C:1
        cpu->clearC();
        testMemory.write(0x2000, 0x6A); // ROR A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x78, cpu->A());
        // EXPECT_EQ(0xF0, cpu->S());
        EXPECT_EQ(0b00100001, cpu->P());
        // EXPECT_EQ(0x09, testMemory.read(0x7235));
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, ROR_AccumulatorWithoutCarryLsb0)
    {
        // --- given
        cpu->A(0xF0); // 0b1111 0000  -> 0111 1000  C:0
        cpu->clearC();
        testMemory.write(0x2000, 0x6A); // ROR A

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2001, cpu->PC());
        EXPECT_EQ(0x78, cpu->A());
        // EXPECT_EQ(0xF0, cpu->S());
        EXPECT_EQ(0b00100000, cpu->P());
        // EXPECT_EQ(0x09, testMemory.read(0x7235));
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::A, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage $LL
    TEST_F(InstructionShiftTest, ROR_ZeroPageWithCarryLsb0)
    {
        // --- given
        cpu->A(0xF0);
        cpu->setC();
        testMemory.write(0x2000, 0x66); // ROR $34
        testMemory.write(0x2001, 0x34);

        testMemory.write(0x0034, 0xD0); // 0b1101 0000 -> 1110 1000 C:0

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF0, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
        EXPECT_EQ(0xE8, testMemory.read(0x0034));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, ROR_ZeroPageWithCarryLsb1)
    {
        // --- given
        cpu->A(0xF0);
        cpu->setC();
        testMemory.write(0x2000, 0x66); // ROR $33
        testMemory.write(0x2001, 0x33);

        testMemory.write(0x0033, 0x41); // 0b0100 0001 -> 1010 0000 C:1

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF0, cpu->A());
        EXPECT_EQ(0b10100001, cpu->P());
        EXPECT_EQ(0xA0, testMemory.read(0x0033));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, ROR_ZeroPageWithoutCarryLsb0)
    {
        // --- given
        cpu->A(0xF0);
        cpu->clearC();
        testMemory.write(0x2000, 0x66); // ROR $34
        testMemory.write(0x2001, 0x34);

        testMemory.write(0x0034, 0xD0); // 0b1101 0000 -> 0110 1000 C:0

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF0, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x68, testMemory.read(0x0034));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    TEST_F(InstructionShiftTest, ROR_ZeroPageWithoutCarryLsb1)
    {
        // --- given
        cpu->A(0xF0);
        cpu->clearC();
        testMemory.write(0x2000, 0x66); // ROR $34
        testMemory.write(0x2001, 0x34);

        testMemory.write(0x0034, 0x11); // 0b0001 0000 -> 0000 1000 C:1

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF0, cpu->A());
        EXPECT_EQ(0b00100001, cpu->P());
        EXPECT_EQ(0x08, testMemory.read(0x0034));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(InstructionShiftTest, ROR_ZeroPageX)
    {
        // --- given
        cpu->A(0xF0);
        cpu->X(0x04);
        cpu->clearC();
        testMemory.write(0x2000, 0x76); // ROR $34,X
        testMemory.write(0x2001, 0x34);

        testMemory.write(0x0038, 0x11); // 0b0001 0001 -> 0000 1000 C:1

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2002, cpu->PC());
        EXPECT_EQ(0xF0, cpu->A());
        EXPECT_EQ(0x04, cpu->X());
        EXPECT_EQ(0b00100001, cpu->P());
        EXPECT_EQ(0x08, testMemory.read(0x0038));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... ZeroPage,Y $LL,Y
    // ..... Relative $BB

    // ----- Absolute $LLHH
    TEST_F(InstructionShiftTest, ROR_Absolute)
    {
        // --- given
        cpu->A(0xF0);
        cpu->X(0x04);
        cpu->clearC();
        testMemory.write(0x2000, 0x6E); // ROR $3442
        testMemory.write(0x2001, 0x42);
        testMemory.write(0x2002, 0x34);

        testMemory.write(0x3442, 0x12); // 0b0001 0010 -> 0000 1001 C:1

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0xF0, cpu->A());
        EXPECT_EQ(0x04, cpu->X());
        EXPECT_EQ(0b00100000, cpu->P());
        EXPECT_EQ(0x09, testMemory.read(0x3442));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(InstructionShiftTest, ROR_AbsoluteX)
    {
        // --- given
        cpu->A(0xF0);
        cpu->X(0x04);
        cpu->clearC();
        testMemory.write(0x2000, 0x7E); // ROR $3442,X
        testMemory.write(0x2001, 0x42);
        testMemory.write(0x2002, 0x34);

        testMemory.write(0x3446, 0x01); // 0b0000 0001 -> 0000 0000 C:1

        // --- when
        cpu->executeFromAddress(0x2000, 1);

        // --- then
        EXPECT_EQ(0x2003, cpu->PC());
        EXPECT_EQ(0xF0, cpu->A());
        EXPECT_EQ(0x04, cpu->X());
        EXPECT_EQ(0b00100011, cpu->P());
        EXPECT_EQ(0x00, testMemory.read(0x3446));
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().src);
        EXPECT_EQ(InstructionTarget::MEMORY, cpu->decodePipeline().dst);
    }

    // ..... AbsoluteY $LLHH,Y
    // ..... Indirect ($LLHH)
    // ..... Indexed Indirect X ($LL,X)
    // ..... Indirect Indexed Y ($LL),Y

    // ----- Flag () -----
    // ----- Comparison () -----
    // ----- Conditional Branch () -----
    // ----- Jump & Subroutine () -----
    // ----- Interrupt () -----
    // ----- Other () -----
}
