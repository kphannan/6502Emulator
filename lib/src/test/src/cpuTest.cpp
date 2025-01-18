

#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class CpuTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("UnitTestMemory");
        memory::Memory testMemory = *tMemory;

        CPU *cpu;

    protected:
        CpuTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            testMemory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            testMemory.write(0x2001, 0x5A); //

            // Reset vector points to start of memory
            testMemory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            testMemory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(testMemory);
        }

        ~CpuTest() override
        {
            delete cpu;
        }
    };

    TEST_F(CpuTest, ConstructDefaultAddressSpace)
    {
        CPU defaultCpu;
        memory::Memory &memory = defaultCpu.currentMemory();

        // --- Make sure there is a full memory by default
        EXPECT_STREQ("DefaultMemory", memory.name());
        EXPECT_EQ(0xFFFF, memory.memorySize());
        EXPECT_EQ(0xFFFF, memory.memorySize());
        EXPECT_EQ(0x0000, memory.lowAddress());
        EXPECT_EQ(0xFFFF, memory.highAddress());

        EXPECT_EQ(0x2000, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0x00, cpu->X());
        EXPECT_EQ(0x00, cpu->Y());
    }

    TEST_F(CpuTest, ConstructTestFixture)
    {
        memory::Memory &memory = cpu->currentMemory();

        // --- Make sure there is a full memory by default
        EXPECT_STREQ("UnitTestMemory", memory.name());
        EXPECT_EQ(0xFFFF, memory.memorySize());
        EXPECT_EQ(0xFFFF, memory.memorySize());
        EXPECT_EQ(0x0000, memory.lowAddress());
        EXPECT_EQ(0xFFFF, memory.highAddress());

        EXPECT_EQ(0x2000, cpu->PC());
        EXPECT_EQ(0x00, cpu->A());
        EXPECT_EQ(0x00, cpu->X());
        EXPECT_EQ(0x00, cpu->Y());
        // EXPECT_EQ(0x20, cpu->P());
        EXPECT_EQ(0b00100000, cpu->P());
    }

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
    // ----- Other () -----

}
