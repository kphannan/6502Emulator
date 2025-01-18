

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class CpuRegisterTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        CpuRegisterTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A); //

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~CpuRegisterTest() override
        {
            delete cpu;
        }
    };

    constexpr hardware::Byte bit0 = 1 << 0; // 0b00000001
    constexpr hardware::Byte bit1 = 1 << 1; // 0b00000010
    constexpr hardware::Byte bit2 = 1 << 2; // 0b00000100
    constexpr hardware::Byte bit3 = 1 << 3; // 0b00001000
    constexpr hardware::Byte bit4 = 1 << 4; // 0b00010000
    constexpr hardware::Byte bit5 = 1 << 5; // 0b00100000
    constexpr hardware::Byte bit6 = 1 << 6; // 0b01000000
    constexpr hardware::Byte bit7 = 1 << 7; // 0b10000000

    // ------------------------------------------------------------------------
    // These tests verify proper bit position of each flag as well as
    // proper set / clear behavior.  Other bits shall not be modified
    // as the result of setting or clearing any specific flag.
    // Bit 5 is always set. Setting and clearing any flag ensures
    // bit 5 remains untouched.
    //
    // ----- (bit 7) Negative
    TEST_F(CpuRegisterTest, FlagsNegativeSet)
    {
        cpu->P() = 0b10000000;

        EXPECT_TRUE(cpu->isN());
    }

    TEST_F(CpuRegisterTest, FlagsNegativeClear)
    {
        cpu->P() = 0b01111111;

        EXPECT_FALSE(cpu->isN());
    }

    TEST_F(CpuRegisterTest, FlagsNegativeBit7)
    {
        cpu->setN();

        EXPECT_EQ(0b10100000, cpu->P());
    }

    // ----- (bit 6) Overflow
    TEST_F(CpuRegisterTest, FlagsOverflowSet)
    {
        cpu->P() = 0b01000000;

        EXPECT_TRUE(cpu->isV());
    }

    TEST_F(CpuRegisterTest, FlagsOverflowClear)
    {
        cpu->P() = 0b10111111;

        EXPECT_FALSE(cpu->isV());
    }

    TEST_F(CpuRegisterTest, FlagsOverflowBit6)
    {
        cpu->setV();

        EXPECT_EQ(0b01100000, cpu->P());
    }

    // ..... (bit 5) (always set)

    // ----- (bit 4) BRK command
    TEST_F(CpuRegisterTest, FlagsBreakSet)
    {
        cpu->P() = 0b00010000;

        EXPECT_TRUE(cpu->isB());
    }

    TEST_F(CpuRegisterTest, FlagsBreakClear)
    {
        cpu->P() = 0b11101111;

        EXPECT_FALSE(cpu->isB());
    }

    TEST_F(CpuRegisterTest, FlagsBreakBit4)
    {
        cpu->setB();

        EXPECT_EQ(0b00110000, cpu->P());
    }

    // ----- (bit 3) Decimal Mode
    TEST_F(CpuRegisterTest, FlagsDecimalModeSet)
    {
        cpu->P() = 0b00001000;

        EXPECT_TRUE(cpu->isD());
    }

    TEST_F(CpuRegisterTest, FlagsDecimalModeClear)
    {
        cpu->P() = 0b11110111;

        EXPECT_FALSE(cpu->isD());
    }

    TEST_F(CpuRegisterTest, FlagsDecimalModeBit3)
    {
        cpu->setD();

        EXPECT_EQ(0b00101000, cpu->P());
    }

    // ----- (bit 2) IRQ Disable
    TEST_F(CpuRegisterTest, FlagsIRQDisableSet)
    {
        cpu->P() = 0b00000100;

        EXPECT_TRUE(cpu->isI());
    }

    TEST_F(CpuRegisterTest, FlagsIRQDisableClear)
    {
        cpu->P() = 0b11111011;

        EXPECT_FALSE(cpu->isI());
    }

    TEST_F(CpuRegisterTest, FlagsIRQDisableBit2)
    {
        cpu->setI();

        EXPECT_EQ(0b00100100, cpu->P());
    }

    // ----- (bit 1) Zero
    TEST_F(CpuRegisterTest, FlagsZeroSet)
    {
        cpu->P() = 0b00000010;

        EXPECT_TRUE(cpu->isZ());
    }

    TEST_F(CpuRegisterTest, FlagsZeroClear)
    {
        cpu->P() = 0b11111101;

        EXPECT_FALSE(cpu->isZ());
    }

    TEST_F(CpuRegisterTest, FlagsZeroBit1)
    {
        cpu->setZ();

        EXPECT_EQ(0b00100010, cpu->P());
    }

    // ----- (bit 0) Carry
    TEST_F(CpuRegisterTest, FlagsCarrySet)
    {
        cpu->P() = 0b00000001;

        EXPECT_TRUE(cpu->isC());
    }

    TEST_F(CpuRegisterTest, FlagsCarryClear)
    {
        cpu->P() = 0b11111110;

        EXPECT_FALSE(cpu->isC());
    }

    TEST_F(CpuRegisterTest, FlagsCarryBit0)
    {
        cpu->setC();

        EXPECT_EQ(0b00100001, cpu->P());
    }

    // ========================================================================
    // ----- Accumulator
    TEST_F(CpuRegisterTest, AccumulatorInitialization)
    {
        EXPECT_EQ(0x00, cpu->A());
    }

    TEST_F(CpuRegisterTest, AccumulatorSetZero)
    {
        cpu->A(0x00);

        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100010, cpu->P());
    }

    TEST_F(CpuRegisterTest, AccumulatorSetNonZero)
    {
        cpu->A(0x42);

        EXPECT_EQ(0x42, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
    }
    TEST_F(CpuRegisterTest, AccumulatorSetNegative)
    {
        cpu->A(0x80);

        EXPECT_EQ(0x80, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
    }

    // ----- Index Register X
    TEST_F(CpuRegisterTest, IndexXInitialization)
    {
        EXPECT_EQ(0x00, cpu->X());
    }

    TEST_F(CpuRegisterTest, IndexXSetZero)
    {
        cpu->A(0x00);

        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100010, cpu->P());
    }

    TEST_F(CpuRegisterTest, IndexXSetNonZero)
    {
        cpu->A(0x42);

        EXPECT_EQ(0x42, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
    }
    TEST_F(CpuRegisterTest, IndexXSetNegative)
    {
        cpu->A(0x80);

        EXPECT_EQ(0x80, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
    }

    // ----- Index Register Y
    TEST_F(CpuRegisterTest, IndexYInitialization)
    {
        EXPECT_EQ(0x00, cpu->Y());
    }

    TEST_F(CpuRegisterTest, IndexYSetZero)
    {
        cpu->A(0x00);

        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0b00100010, cpu->P());
    }

    TEST_F(CpuRegisterTest, IndexYSetNonZero)
    {
        cpu->A(0x42);

        EXPECT_EQ(0x42, cpu->A());
        EXPECT_EQ(0b00100000, cpu->P());
    }
    TEST_F(CpuRegisterTest, IndexYSetNegative)
    {
        cpu->A(0x80);

        EXPECT_EQ(0x80, cpu->A());
        EXPECT_EQ(0b10100000, cpu->P());
    }

    // ----- Stack Pointer
    TEST_F(CpuRegisterTest, StackPointerInitialization)
    {
        EXPECT_EQ(0x00, cpu->S()); // TODO should be $1FF
    }

}
