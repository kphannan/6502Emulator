

#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "memory.hpp"

namespace memory
{
    class MemoryTest : public testing::Test
    {
    public:
        memory::Memory memory = *(new memory::Memory( "UnitTest - Memory" ));

    protected:
        MemoryTest() //: memory(*(new Memory("test suite")))
        {
            memory.fill(0xAA);
        }
    };

    TEST_F(MemoryTest, ConstructDefaultAddressSpace)
    {
        // --- given
        // --- when
        Memory testBank;

        // --- then
        // EXPECT_TRUE(nullptrptr != memory);
        EXPECT_STREQ("DefaultMemory", testBank.name());
        EXPECT_EQ(0x10000, testBank.memorySize());
        EXPECT_EQ(0x0000,  testBank.lowAddress());
        EXPECT_EQ(0xFFFF,  testBank.highAddress());
    }

    TEST_F(MemoryTest, ConstructNamedAddressSpaceDefaultSize)
    {
        // --- given
        // --- when
        Memory testBank( "TestMem" );

        // --- then
        // EXPECT_TRUE(nullptrptr != memory);
        EXPECT_STREQ("TestMem", testBank.name());
        EXPECT_EQ(0x10000, testBank.memorySize());
        EXPECT_EQ(0x0000, testBank.lowAddress());
        EXPECT_EQ(0xFFFF, testBank.highAddress());
    }

    TEST_F(MemoryTest, ConstructAddressSpaceWithSize)
    {
        // --- given
        // --- when
        Memory testBank( 0x3FF );

        // --- then
        EXPECT_STREQ("DefaultMemory", testBank.name());
        EXPECT_EQ(0x0400, testBank.memorySize());
        EXPECT_EQ(0x0000, testBank.lowAddress());
        EXPECT_EQ(0x03FF, testBank.highAddress());
    }

    TEST_F(MemoryTest, ConstructNamedAddressSpaceWithSize)
    {
        // --- given
        // --- when
        Memory testBank( "ThreePages", 0x03FF);

        // --- then
        EXPECT_STREQ("ThreePages", testBank.name());
        EXPECT_EQ(0x0400, testBank.memorySize());
        EXPECT_EQ(0x0000, testBank.lowAddress());
        EXPECT_EQ(0x03FF, testBank.highAddress());
    }

    TEST_F(MemoryTest, ConstructNamedAddressSpaceWithBounds)
    {
        // --- given
        // --- when
        Memory testMemory("0x8000-0x9fff", hardware::Address(0x8000), hardware::Address(0x9FFF));

        // --- then
        // EXPECT_TRUE(nullptrptr != memory);
        EXPECT_STREQ("0x8000-0x9fff", testMemory.name());
        EXPECT_EQ(0x2000, testMemory.memorySize()); // 0x9FFF - 0x8000 + 1
        EXPECT_EQ(0x8000, testMemory.lowAddress());
        EXPECT_EQ(0x9FFF, testMemory.highAddress());
    }

    // ===== Memory Access =====

    // --- Boundaries
    TEST_F(MemoryTest, AccessDefaultMemoryAtBounds)
    {
        // --- given
        // --- when
        memory.showMemory(0x0000, 0x08, "Start of memory");
        memory.showMemory(0xFFF8, 0x08, "End of memory");

        // --- then
        EXPECT_EQ(0xAA, memory[0x0000]);
        EXPECT_EQ(0xAA, memory[0xFFFF]);
    }

    TEST_F(MemoryTest, AccessMemoryNotStartingAtZeroMemoryAtBounds)
    {
        // --- given
        // --- when
        Memory testMemory("0x8000-0x9fff",
                          hardware::Address(0x8000), hardware::Address(0x9FFF));
        testMemory.fill(0xCC);

        // --- then
        EXPECT_EQ(0xCC, testMemory.read(0x8000));
        EXPECT_EQ(0xCC, testMemory.read(0x9FFF));
    }

    // --- write (byte)
    TEST_F(MemoryTest, WriteByteAtStartOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory.read(0x0000));

        // --- when
        memory.write(0x0000, 0x55);

        // --- then
        EXPECT_EQ(0x55, memory[0x0000]);
        EXPECT_EQ(0xAA, memory[0x0001]);
    }

    TEST_F(MemoryTest, WriteByteAtEndOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory[0xFFFF]);

        // --- when
        memory.write(0xFFFF, 0x77);

        // --- then
        EXPECT_EQ(0x77, memory[0xFFFF]);
        EXPECT_EQ(0xAA, memory[0xFFFE]);
    }

    // --- write (word)
    TEST_F(MemoryTest, WriteWordAtStartOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAAAA, memory.readWord(0x0000));
        // 0x0000: AA AA AA AA AA AA AA AA

        // --- when
        memory.writeWord(0x0000, 0x5511);

        // 0x0000: 11 55 AA AA AA AA AA AA

        // --- then
        memory.showMemory(0x0000, 0x08);
        EXPECT_EQ(0x5511, memory.readWord(0x0000));
        EXPECT_EQ(0xAAAA, memory.readWord(0x0002));
        EXPECT_EQ(0xAA55, memory.readWord(0x0001));
    }

    TEST_F(MemoryTest, WriteWordAtEndOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAAAA, memory.readWord(0xFFFE));
        // 0xFFF8: AA AA AA AA AA AA AA AA

        // --- when
        memory.writeWord(0xFFFE, 0x7788);
        // 0xFFF8: AA AA AA AA AA AA 88 77

        // --- then
        memory.showMemory(0xFFF0, 0x0F);
        EXPECT_EQ(0x7788, memory.readWord(0xFFFE));
        EXPECT_EQ(0xAAAA, memory.readWord(0xFFFC));
        EXPECT_EQ(0x88AA, memory.readWord(0xFFFD));
    }

    TEST_F(MemoryTest, AccessMemoryNotStartingAtZeroMemoryBelowLowerBound)
    {
        // --- given
        // --- when
        Memory memoryBank("0x8000-0x9fff",
                          hardware::Address(0x8000), hardware::Address(0x9FFF));

        // --- then
        EXPECT_STREQ("0x8000-0x9fff", memoryBank.name());

        EXPECT_THROW(memoryBank.read(0x0000), std::out_of_range);
        EXPECT_THROW(memoryBank.read(0x7FFF), std::out_of_range);
        EXPECT_THROW(memoryBank.read(0xA000), std::out_of_range);
        EXPECT_THROW(memoryBank[0x0000], std::out_of_range);
        EXPECT_THROW(memoryBank[0x7FFF], std::out_of_range);
        EXPECT_THROW(memoryBank[0xA000], std::out_of_range);
    }

    /// --- operator []
    TEST_F(MemoryTest, ArrayOperatorAtStartOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory[0x0000]);

        // --- when
        memory[0x0000] = 0x77;

        // --- then
        EXPECT_EQ(0x77, memory[0x0000]);
        EXPECT_EQ(0xAA, memory[0x0001]);
    }

    TEST_F(MemoryTest, ArrayOperatorAtEndOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory.read(0xFFFF));

        // --- when
        memory.write(0xFFFF, 0x77);

        // --- then
        EXPECT_EQ(0x77, memory.read(0xFFFF));
        EXPECT_EQ(0xAA, memory.read(0xFFFE));
    }

}
